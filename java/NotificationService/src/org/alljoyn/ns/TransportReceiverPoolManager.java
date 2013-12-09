/******************************************************************************
 * Copyright (c) 2013, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

package org.alljoyn.ns;

import java.util.concurrent.RejectedExecutionException;
import java.util.concurrent.RejectedExecutionHandler;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 *  The thread pool manager, is used to return the received notifications to
 *  an application writer
 */
class TransportReceiverPoolManager implements RejectedExecutionHandler {
	
	/**
	 * The thread TTL time
	 */
	private static final long THREAD_TTL        = 3;
	
	/**
	 * The thread TTL time unit
	 */
	private static final TimeUnit TTL_TIME_UNIT =  TimeUnit.SECONDS;
	
	/**
	 * Amount of the core threads 
	 */
	private static final int THREAD_CORE_NUM    = 4;
	
	/**
	 * The maximum number of threads that may be spawn
	 */
	private static final int THREAD_MAX_NUM     = 10;
	
	/**
	 * Thread pool object
	 */
	private ThreadPoolExecutor threadPool;

	/**
	 * Constructor 
	 * Starts the thread pool
	 */
	public TransportReceiverPoolManager() {
		initPool();
	}
	
	/**
	 * @param task Execute the given runnable task if there is a free thread worker
	 */
	public void execute(Runnable task) {
		threadPool.execute(task);
	}//execute
	
	/**
	 * Tries to stop the thread pool and all its threads  
	 */
	public void shutdown() {
		if ( threadPool == null || threadPool.isShutdown() ) {
			return;
		}
		
		threadPool.shutdown();
		threadPool = null;
	}//shutdown
	
	/**
	 * The callback is called, when there are no free threads to execute the given task  
	 */
	@Override
	public void rejectedExecution(Runnable r, ThreadPoolExecutor executor) {
		throw new RejectedExecutionException("Failed to execute the given task, all the worker threads are busy");
	}//rejectedExecution
	
	/**
	 * Initializes the thread pool
	 */
	private void initPool() {
		threadPool = new ThreadPoolExecutor(
				THREAD_CORE_NUM,
				THREAD_MAX_NUM,
				THREAD_TTL,
				TTL_TIME_UNIT,
				new SynchronousQueue<Runnable>(true) // The fair order FIFO
		);
		threadPool.setRejectedExecutionHandler(this);
	}//initPool
}
