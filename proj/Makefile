PROG=proj

SRCS = proj.c proj_aux.c video_gr.c keyboard.c utils.c sprite.c mouse.c timer.c rtc.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
