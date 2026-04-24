# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    channel.mk                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/23 17:32:03 by jaubry--          #+#    #+#              #
#    Updated: 2026/04/23 17:33:08 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
CHANNEL_DIR      = $(COMMANDS_DIR)/channel

# Source files
CHANNEL_SRCS     = JoinCommand.cpp \
				   PrivmsgCommand.cpp

SRCS            += $(addprefix $(CHANNEL_DIR)/, $(CHANNEL_SRCS))


# VPATH
vpath %.cpp $(CHANNEL_DIR)
