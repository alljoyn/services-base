/******************************************************************************
 * Copyright (c) 2013-2014, AllSeen Alliance. All rights reserved.
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


#ifndef ASYNCTASK_H_
#define ASYNCTASK_H_

#include <pthread.h>
#include <queue>
#include <qcc/String.h>

namespace ajn {
namespace services {

/**
 * class AsyncTaskEvents
 * User must derived from this class and implement its virtaul functions
 */
template <class TaskData>
class AsyncTaskEvents {
  public:
    AsyncTaskEvents() { }
    virtual ~AsyncTaskEvents() { };
    /**
     * OnEmptyQueue - handling 'queue become empty' events.
     */
    virtual void OnEmptyQueue() { }
    /**
     * OnEmptyQueue - handling 'queue got new message' events.
     */
    virtual void OnGotMessage(TaskData const& taskData) { }
};

/**
 * class AsyncTask gives services for processing asynchronous tasks.
 */
template <class TaskData>
class AsyncTask {
  public:
    /**
     * AsyncTask constructor
     */
    AsyncTask(AsyncTaskEvents<TaskData>* pAsyncTaskEvents);
    /**
     * AsyncTask destructor
     */
    ~AsyncTask();
    /**
     * Start
     */
    void Start();
    /**
     * Stop
     */
    void Stop();
    /**
     * HandleTask
     */
    void HandleTask(TaskData& taskdata);
  private:
    /**
     * The thread responsible for receiving messages
     */
    pthread_t m_ReceiverThread;

    /**
     * A Queue that holds the messages
     */
    std::queue<TaskData> m_MessageQueue;

    /**
     * The mutex Lock
     */
    pthread_mutex_t m_Lock;

    /**
     * The Queue Changed thread condition
     */
    pthread_cond_t m_QueueChanged;

    /**
     * is the thread in the process of shutting down
     */
    bool m_IsStopping;

    /**
     * A wrapper for the receiver Thread
     * @param context
     */
    static void* ReceiverThreadWrapper(void* context);

    /**
     * The function run in the Receiver
     */
    void Receiver();
    /**
     * Tag for logging
     */
    qcc::String TAG;

    /**
     * pointer to client to report about events
     */
    AsyncTaskEvents<TaskData>* m_AsyncTaskEvents;

};

} //namespace services
} //namespace ajn


using namespace ajn;
using namespace services;
using namespace qcc;

template <class TaskData>
AsyncTask<TaskData>::AsyncTask(AsyncTaskEvents<TaskData>* pAsyncTaskEvents) :
    m_IsStopping(true), m_AsyncTaskEvents(pAsyncTaskEvents)
{

}

template <class TaskData>
AsyncTask<TaskData>::~AsyncTask()
{

}

template <class TaskData>
void AsyncTask<TaskData>::HandleTask(TaskData& taskdata)
{
    pthread_mutex_lock(&m_Lock);
    m_MessageQueue.push(taskdata);
    pthread_cond_signal(&m_QueueChanged);
    pthread_mutex_unlock(&m_Lock);
}

template <class TaskData>
void AsyncTask<TaskData>::Start()
{
    if (!m_IsStopping)
        return;

    m_IsStopping = false;

    pthread_mutex_init(&m_Lock, NULL);
    pthread_cond_init(&m_QueueChanged, NULL);
    pthread_create(&m_ReceiverThread, NULL, ReceiverThreadWrapper, this);
}

template <class TaskData>
void AsyncTask<TaskData>::Stop()
{
    if (m_IsStopping)
        return;

    pthread_mutex_lock(&m_Lock);
    while (!m_MessageQueue.empty()) {
        m_MessageQueue.pop();
    }
    m_IsStopping = true;
    pthread_cond_signal(&m_QueueChanged);
    pthread_mutex_unlock(&m_Lock);
    pthread_join(m_ReceiverThread, NULL);

    pthread_cond_destroy(&m_QueueChanged);
    pthread_mutex_destroy(&m_Lock);
}

template <class TaskData>
void* AsyncTask<TaskData>::ReceiverThreadWrapper(void* context)
{
    AsyncTask* asyncTask = reinterpret_cast<AsyncTask*>(context);
    if (asyncTask == NULL) // should not happen
        return NULL;
    asyncTask->Receiver();
    return NULL;
}

template <class TaskData>
void AsyncTask<TaskData>::Receiver()
{
    pthread_mutex_lock(&m_Lock);
    while (!m_IsStopping) {
        while (!m_MessageQueue.empty()) {
            TaskData const& taskData = m_MessageQueue.front();
            //Do action - new data
            if (m_AsyncTaskEvents)
                m_AsyncTaskEvents->OnGotMessage(taskData);
            m_MessageQueue.pop();
            pthread_mutex_unlock(&m_Lock);

            pthread_mutex_lock(&m_Lock);
        }
        //Do action - queue is empty
        if (m_AsyncTaskEvents)
            m_AsyncTaskEvents->OnEmptyQueue();
        pthread_cond_wait(&m_QueueChanged, &m_Lock);
    }
    pthread_mutex_unlock(&m_Lock);
}


#endif /* ASYNCTASK_H_ */
