extern int shell_row;

void shell_print(const char *text);
void shell_run();
void shell_clear(void);

extern int shell_active;

void shell_set_active(int active);
int shell_is_active(void);

