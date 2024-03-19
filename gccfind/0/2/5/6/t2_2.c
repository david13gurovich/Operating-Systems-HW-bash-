#include <unistd.h>
#include <stdio.h>

int main(void)
{
    int status;
    char *argv[] = { "/bin/env", 0 };
    char *envp[] =
    {
        "HOME=/",
        "PATH=/bin:/usr/bin",
        "TZ=UTC0",
        "USER=beelzebub",
        "LOGNAME=tarzan",
        0
    };
    status=execve(argv[0], &argv[0], envp);
    printf("exec failed\n");
    return -1;
}
