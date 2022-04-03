//
// Created by Frityet on 2022-04-03.
//

#pragma once

#include "wurstlink_types.h"

#include <pthread.h>

typedef void OnJobComplete_f(void);
typedef void WhileIdle_f(void);

typedef void (nonnull_ptr TaskAction_pf)(int argc, const void *argv);

struct Task {
    TaskAction_pf   action;
    struct Task     *next;
};

struct TaskQueue {
    size_t      task_count;
    struct Task *head;
};

struct Worker {
    pthread_t           thread;

    OnJobComplete_f     *job_complete;
    WhileIdle_f         *while_idle;
    struct TaskQueue    work_queue;
};

struct Worker Worker(OnJobComplete_f *complete, WhileIdle_f *idle);
void Worker$add_task(struct Worker *worker, struct Task nonnull_ptr task);
void Worker$merge(struct Worker *worker);

struct Task *Task(TaskAction_pf func);
