import re

with open('mquickjs/mquickjs.c', 'r') as f:
    content = f.read()

# Remove the WASI PATCHED comment and the guards
content = content.replace("/* WASI PATCHED */\n", "")
# Use a more robust regex to remove the guards I added
content = re.sub(r'#ifndef __wasi__\n#include <sys/wait\.h>\n#include <signal\.h>\n#endif\n',
                 '#include <sys/wait.h>\n#include <signal.h>\n', content)

with open('mquickjs/mquickjs.c', 'w') as f:
    f.write(content)
