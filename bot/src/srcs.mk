# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    srcs.mk                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/14 17:51:47 by jaubry--          #+#    #+#              #
#    Updated: 2026/05/06 17:29:09 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Source files
BOT_SRCS	= main.cpp \
			  Bot.cpp \
			  BotCommand.cpp \
			  BotSignals.cpp \
			  BotReply.cpp
SRCS		+= $(addprefix $(SRCDIR)/, $(BOT_SRCS))

# VPATH
vpath %.cpp	$(SRCDIR)
