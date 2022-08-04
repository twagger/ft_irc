# PROGRAM INFOS
################################################################################
VERSION		= "1.0"
VCOMMENT	= "This version is almost perfect."
SERVERINFO  = "Every time you start this server, we plant a tree 🌲."
HOSTNAME	= $(shell hostname)
COMPILDATE	= "$(shell date)"

# COMMANDS
################################################################################
RM			= rm -f
RMRF		= rm -rf
CC			= c++
CD			= cd
MKDIR		= mkdir
GCLONE		= git clone

# SOURCES
################################################################################
SRCS		= srcs/main.cpp \
			  srcs/server/Server.cpp \
		      srcs/server/exceptions.cpp \
			  srcs/channel/channel.cpp \
			  srcs/commands/join.cpp \
			  srcs/commands/part.cpp \
			  srcs/commands/invite.cpp \
			  srcs/commands/kick.cpp \
			  srcs/commands/topic.cpp \
			  srcs/commands/list.cpp \
			  srcs/commands/names.cpp \
			  srcs/user/User.cpp \
			  srcs/commands/cap.cpp \
			  srcs/commands/die.cpp \
			  srcs/commands/info.cpp \
			  srcs/commands/kill.cpp \
			  srcs/commands/mode.cpp \
			  srcs/commands/motd.cpp \
			  srcs/commands/nick.cpp \
			  srcs/commands/oper.cpp \
			  srcs/commands/pass.cpp \
			  srcs/commands/ping.cpp \
			  srcs/commands/pong.cpp \
			  srcs/commands/quit.cpp \
			  srcs/commands/time.cpp \
			  srcs/commands/user.cpp \
			  srcs/commands/privmsg.cpp \
			  srcs/commands/notice.cpp \
			  srcs/commands/version.cpp \
			  srcs/commands/who.cpp \
			  srcs/commands/whois.cpp \
			  srcs/utils/errors.cpp \
			  srcs/utils/parsing.cpp \
			  srcs/utils/welcome.cpp \
			  srcs/utils/command_utils.cpp \
			  srcs/parser/mask.cpp
OBJS		= $(SRCS:.cpp=.o)

# EXECUTABLES & LIBRARIES
################################################################################
NAME		= ircserv

# DIRECTORIES
################################################################################
HEADERS		= -Iincludes -Isrcs/server

# FLAGS
################################################################################
CPPFLAGS		:= -Wall -Wextra -Werror -std=c++98 -pedantic -g3

PROGRAMVAR		:= -DHOSTNAME=\"$(HOSTNAME)\" -DVERSION=\"$(VERSION)\" \
				   -DVCOMMENT=\"$(VCOMMENT)\" -DCOMPILDATE=\"$(COMPILDATE)\"
				   -DSERVERINFO=\"$(SERVERINFO)\"
				   
ifeq ($(DEBUG), true)
	CPPFLAGS	+= -fsanitize=address -g3 -O0
endif

ifeq ($(SILENT), true)
	CPPFLAGS	+= -DSILENCE
endif

# RULES
################################################################################
.cpp.o:
			$(CC) $(CPPFLAGS) -c $< -o $(<:.cpp=.o) $(HEADERS) $(PROGRAMVAR)

$(NAME):	$(OBJS)
			$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME) $(HEADERS)

all:		$(NAME)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean cpp.o re
