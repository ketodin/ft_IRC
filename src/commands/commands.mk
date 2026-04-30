# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    commands.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/23 17:26:29 by jaubry--          #+#    #+#              #
#    Updated: 2026/04/30 00:52:49 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories

COMMANDS_DIR	= $(SRCDIR)/commands

COMMANDS_SRCS	= ACommand.cpp \
				  ACommandException.cpp \
				  CommandParser.cpp \
				  CommandDispatcher.cpp
SRCS			+= $(addprefix $(COMMANDS_DIR)/, $(COMMANDS_SRCS))

COMMANDS_MKS	= auth/auth.mk \
				  channel/channel.mk \
				  operator/operator.mk

include $(addprefix $(COMMANDS_DIR)/, $(COMMANDS_MKS))

# VPATH
vpath %.cpp $(COMMANDS_DIR)
