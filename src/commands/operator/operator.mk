# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    operator.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/23 17:33:21 by jaubry--          #+#    #+#              #
#    Updated: 2026/04/23 17:33:59 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
OPERATOR_DIR      = $(COMMANDS_DIR)/operator

# Source files
OPERATOR_SRCS     = InviteCommand.cpp \
					KickCommand.cpp \
					ModeCommand.cpp \
					TopicCommand.cpp

SRCS            += $(addprefix $(OPERATOR_DIR)/, $(OPERATOR_SRCS))


# VPATH
vpath %.cpp $(OPERATOR_DIR)
