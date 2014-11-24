#    Copyright (c) 2014, AllSeen Alliance. All rights reserved.
#
#       Permission to use, copy, modify, and/or distribute this software for any
#       purpose with or without fee is hereby granted, provided that the above
#       copyright notice and this permission notice appear in all copies.
#
#       THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
#       WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
#       MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
#       ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
#       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
#       ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
#       OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

$colItems = Get-WmiObject Win32_NetworkAdapterConfiguration -Namespace "root\CIMV2" -ComputerName $strComputer | where{$_.IPEnabled -eq "True"}
#$objItem = $colItems[0]
foreach($objItem in $colItems) {
 If($objItem.ServiceName -Match "vwifimp") {
#Write-Host "ServiceName:" $objItem.ServiceName
#Write-Host "Caption:" $objItem.Caption
#Write-Host "DHCP Enabled:" $objItem.DHCPEnabled
#Write-Host "IP Address:" $objItem.IPAddress
#Write-Host "Subnet Mask:" $objItem.IPSubnet
#Write-Host "Gateway:" $objItem.DefaultIPGateway
Write-Host $objItem.MACAddress
}
}
