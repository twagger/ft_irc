# PROGRAM INFOS
################################################################################
VERSION		= "0.1"
VCOMMENT	= "This version is almost perfect."
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
			  srcs/channel/channel.cpp \
			  srcs/channel/join.cpp \
			  srcs/channel/part.cpp \
			  srcs/channel/invite.cpp \
			  srcs/channel/kick.cpp \
			  srcs/channel/topic.cpp \
			  srcs/channel/list.cpp \
			  srcs/channel/names.cpp \
			  srcs/user/User.cpp \
			  srcs/commands/kill.cpp \
			  srcs/commands/nick.cpp \
			  srcs/commands/pass.cpp \
			  srcs/commands/ping.cpp \
			  srcs/commands/pong.cpp \
			  srcs/commands/motd.cpp \
			  srcs/commands/cap.cpp \
			  srcs/commands/version.cpp \
			  srcs/commands/time.cpp \
			  srcs/commands/info.cpp \
			  srcs/commands/quit.cpp \
			  srcs/commands/user.cpp \
			  srcs/commands/privmsg.cpp \
			  srcs/utils/errors.cpp \
			  srcs/utils/parsing.cpp \
			  srcs/utils/welcome.cpp \
			  srcs/utils/command_utils.cpp 			  
OBJS		= $(SRCS:.cpp=.o)

# EXECUTABLES & LIBRARIES
################################################################################
NAME		= ircserv

# DIRECTORIES
################################################################################
HEADERS		= -Iincludes -Isrcs/server

# FLAGS
################################################################################
CPPFLAGS		:= -Wall -Wextra -Werror -std=c++98 -pedantic -g3 -fsanitize=address

PROGRAMVAR		:= -DHOSTNAME=\"$(HOSTNAME)\" -DVERSION=\"$(VERSION)\" \
				   -DVCOMMENT=\"$(VCOMMENT)\" -DCOMPILDATE=\"$(COMPILDATE)\"
				   
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
