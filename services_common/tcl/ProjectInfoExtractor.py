# Copyright (c) 2013, AllSeen Alliance. All rights reserved.
#
#    Permission to use, copy, modify, and/or distribute this software for any
#    purpose with or without fee is hereby granted, provided that the above
#    copyright notice and this permission notice appear in all copies.
#
#    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
#    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
#    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
#    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
#    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
#    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
#    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#!/usr/bin/python
# -*- coding: utf-8 -*-

import SCons
import os.path
import re
import subprocess
import sys


def get_user_includes(environ):
    """Returns all user includes in the given environment."""

    # '#/a/b/c' and '#a/b/c' both need to be 'a/b/c'
    re_sconstruct_dir = re.compile(r'(#[%s]?)?(.*)' % re.escape(os.path.sep))

    def normalize(path):
        sconstruct_dir = re_sconstruct_dir.split(path)
        return path if len(sconstruct_dir) < 3 else os.path.abspath(sconstruct_dir[2])

    includes = set()
    for path in environ['CPPPATH']:
        if isinstance(path, str):
            new_path = normalize(environ.subst(path))
            if os.path.isdir(new_path):
                includes.add(new_path)
        else:
            includes.add(normalize(path.srcnode().abspath))
    return includes


def get_gcc_lang_param(lang):
    return ('-xc++' if lang == 'c++' else '-xc')


def get_sys_includes(lang, environ):
    """Returns all system includes of compiler collection for the given language."""

    def create_compiler_input(lang):
        in_file = os.path.join(SCons.Script.Dir('.').abspath, '.in')
        with open(in_file, 'w') as f:
            f.write('#include <%s>\nint main(){}' % (('cstdlib' if lang
                    == 'c++' else 'stdlib.h')))
        return in_file

    def invoke_compiler(lang, environ):
        in_file = create_compiler_input(lang)
        out_file = os.path.join(SCons.Script.Dir('.').abspath, '.a.out')
        process = SCons.Action._subproc(environ, [
            get_compiler(environ),
            '-v',
            get_gcc_lang_param(lang),
            in_file,
            '-o',
            out_file,
            ], stdin='devnull', stderr=subprocess.PIPE,
                stdout=subprocess.PIPE)
        (_, perr) = process.communicate()
        
        if os.path.exists(in_file):
            os.remove(in_file)
        if os.path.exists(out_file):
            os.remove(out_file)
        
        return perr

    def parse_includes(cc_output):
        re_incl = re.compile('#include <\.\.\.>.*:$\s((^ .*\s)*)', re.M)
        match = re_incl.search(cc_output)
        sysincludes = set()
        if match:
            for it in re.finditer('^ (.*)$', match.group(1), re.M):
                sysincludes.add(it.groups()[0])
        return sysincludes

    cc_output = invoke_compiler(lang, environ)
    return parse_includes(cc_output)


def get_compiler(environ):
    return ('g++-4' if environ['PLATFORM'] == 'win32' else environ['CXX'])

 
def get_macros_from_cpp_defines(environ):
    """Returns all macros in CPPDEFINES of the given environment."""
    macros = set()
    
    for macro in environ['CPPDEFINES']:
        if isinstance(macro, (list, tuple)) and len(macro) > 1:
            macros.add('%s=%s' % (macro[0], macro[1]))
        elif macro is not None:
            macros.add(macro)
    
    return macros


def get_macros_from_cc_flags(environ):
    """Returns all macros which are defined in CCFLAGS of the given environment."""

    macros = [flag[2:] for flag in environ['CCFLAGS'] if flag
              is not None and flag.startswith('-D')]
    return set(macros)


def get_sys_macros(lang, environ):
    """Returns all predefined macros of compiler collection for the given lang."""

    process = SCons.Action._subproc(environ, [get_compiler(environ), '-E', '-dM',
                                 get_gcc_lang_param(lang), '-'],
                                 stdin='devnull',
                                 stderr=subprocess.PIPE,
                                 stdout=subprocess.PIPE)
    (pout, _) = process.communicate()
    sysmacros = set()

    for it in re.finditer('^#define (.*) (.*)$', pout, re.M):
        sysmacros.add('%s=%s' % (it.groups()[0], it.groups()[1]))

    return sysmacros


def has_build_targets():
    return len(SCons.Script.BUILD_TARGETS) > 0


def get_start_nodes():
    if has_build_targets():
        return SCons.Script.Alias(SCons.Script.BUILD_TARGETS)
    return [SCons.Script.Dir('.')]


def write_config(includes, macros, environ):

    def to_string(objects):
        return ','.join([("'%s'" % '='.join(obj) if isinstance(obj,
                        tuple) else "'%s'" % obj) for obj in objects])

    with open('SConfig', 'w') as f:
        f.write('USER_INCLUDES = %s\n' % to_string(includes))
        f.write('SYS_C_INCLUDES = %s\n'
                % to_string(get_sys_includes('cc', environ)))
        f.write('SYS_CPP_INCLUDES = %s\n'
                % to_string(get_sys_includes('c++', environ)))
        f.write('MACROS = %s\n' % to_string(macros))
        f.write('SYS_C_MACROS = %s\n' % to_string(get_sys_macros('cc',
                environ)))
        f.write('SYS_CPP_MACROS = %s\n' % to_string(get_sys_macros('c++'
                , environ)))


def gather_information(super_nodes):

    def no_scan_fun(node, _):
        return node.children(scan=0)

    children_fun = (no_scan_fun if has_build_targets() else SCons.Node.get_children)
    includes = set()
    macros = set()
    compiler_environ = None

    for super_node in super_nodes:
        walker = SCons.Node.Walker(super_node, kids_func=children_fun)
        node = walker.get_next()
        while node:
            if node and node.has_builder():
                environ = node.get_build_env()
                
                if 'CPPPATH' in environ:
                    includes.update(get_user_includes(environ))
                if 'CPPDEFINES' in environ:
                    macros.update(get_macros_from_cpp_defines(environ))
                if 'CCFLAGS' in environ:
                    macros.update(get_macros_from_cc_flags(environ))
                if not compiler_environ and ('CC' in environ or 'CXX' in environ):
                    compiler_environ = environ
            node = walker.get_next()

    return (includes, macros, compiler_environ)


(incls, macs, env) = gather_information(get_start_nodes())
write_config(incls, macs, env)

print "Done collecting project information."
sys.exit()
