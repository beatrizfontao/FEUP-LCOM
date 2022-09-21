# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = proj.c graphic.c keyboard.c menu.c utils.c game.c mouse.c ship.c shipBullet.c alienBullet.c timer.c alien.c rtc.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -D __LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
