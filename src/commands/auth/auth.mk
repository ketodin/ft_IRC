# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    auth.mk                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ekeisler <ekeisler@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/23 17:30:48 by jaubry--          #+#    #+#              #
#    Updated: 2026/04/27 17:45:45 by ekeisler         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
AUTH_DIR      = $(COMMANDS_DIR)/auth

# Source files
AUTH_SRCS     = PassCommand.cpp \
				NickCommand.cpp \
				UserCommand.cpp \
				CapCommand.cpp

SRCS            += $(addprefix $(AUTH_DIR)/, $(AUTH_SRCS))


# VPATH
vpath %.cpp $(AUTH_DIR)
