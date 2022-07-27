
HOSTNAME	=  $(shell hostname)

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
			  srcs/utils/errors.cpp \
			  srcs/utils/parsing.cpp \
			  srcs/channel/channel.cpp \
			  srcs/channel/join.cpp \
			  srcs/channel/part.cpp \
			  srcs/channel/invite.cpp \
			  srcs/channel/kick.cpp \
			  srcs/utils/command_utils.cpp \
			  srcs/user/user.cpp \
			  srcs/commands/kill.cpp \
			  srcs/user/usercmds.cpp 
			  
OBJS		= $(SRCS:.cpp=.o)

# EXECUTABLES & LIBRARIES
################################################################################
NAME		= server

# DIRECTORIES
################################################################################
HEADERS		= -Iincludes -Isrcs/server

# FLAGS
################################################################################
CPPFLAGS		:= -Wall -Wextra -Werror -std=c++98 -pedantic

ifeq ($(DEBUG), true)
	CPPFLAGS	+= -fsanitize=address -g3 -O0
endif

ifeq ($(SILENT), true)
	CPPFLAGS	+= -DSILENCE
endif

# RULES
################################################################################
.cpp.o:
			$(CC) $(CPPFLAGS) -c $< -o $(<:.cpp=.o) $(HEADERS) \
			-DHOSTNAME=\"$(HOSTNAME)\"

$(NAME):	$(OBJS)
			$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME) $(HEADERS)

all:		$(NAME)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean cpp.o re