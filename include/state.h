#ifndef STATE_H
#define STATE_H

#include <stdarg.h>
#include <pthread.h>
#include <time.h>
#include <libtsm.h>
#include <poll.h>

#include "bind.h"
#include "util/list.h"
#include "worker.h"

enum account_status {
	ACCOUNT_NOT_READY,
	ACCOUNT_OKAY,
	ACCOUNT_ERROR
};

struct geometry {
	int x, y, width, height;
};

struct message_renderer {
	pid_t pid;
	int pipe[2];
	struct pollfd poll[1];
	size_t output_len;
	size_t output_size;
	uint8_t *output;
	size_t input_size;
	uint8_t *input;
};

struct account_state {
	struct {
		struct worker_pipe *pipe;
		pthread_t thread;
	} worker;

	struct {
		char *text;
		struct timespec since;
		enum account_status status;
	} status;

	struct {
		size_t selected_message;
		size_t list_offset;
		list_t *fetch_requests;
	} ui;
	
	struct {
		struct aerc_message *msg;
		struct subprocess *term;
		list_t *processes;
	} viewer;

	char *name;
	list_t *mailboxes;
	char *selected;
	struct account_config *config;
};

struct aerc_state {
	size_t selected_account;
	list_t *accounts;
	bool exit;
	unsigned int rerender;
	struct {
		char *text;
		size_t length, index, scroll;
		list_t *cmd_history;
	} command;
	struct {
		struct geometry client;
		struct geometry account_tabs;
		struct geometry sidebar;
		struct geometry message_list;
		struct geometry message_view;
		struct geometry status_bar;
	} panels;
	struct bind *lbinds;
	struct bind *mbinds;
};

extern struct aerc_state *state;

void set_status(struct account_state *account, enum account_status state,
		const char *fmt, ...);
struct aerc_mailbox *get_aerc_mailbox(struct account_state *account,
		const char *name);
void free_aerc_mailbox(struct aerc_mailbox *mbox);
void free_aerc_message(struct aerc_message *msg);
const char *get_message_header(struct aerc_message *msg, char *key);
bool get_message_flag(struct aerc_message *msg, char *flag);
bool get_mailbox_flag(struct aerc_mailbox *mbox, char *flag);
struct account_config *config_for_account(const char *name);

#endif
