# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    channel.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/23 17:32:03 by jaubry--          #+#    #+#              #
#    Updated: 2026/04/29 00:40:13 by ekeisler         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
CHANNEL_DIR      = $(COMMANDS_DIR)/channel

# Source files
CHANNEL_SRCS     = JoinCommand.cpp \
				   PrivmsgCommand.cpp \
				   PongCommand.cpp

SRCS            += $(addprefix $(CHANNEL_DIR)/, $(CHANNEL_SRCS))


# VPATH
vpath %.cpp $(CHANNEL_DIR)
