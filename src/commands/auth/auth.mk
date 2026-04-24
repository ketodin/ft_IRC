# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    auth.mk                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/23 17:30:48 by jaubry--          #+#    #+#              #
#    Updated: 2026/04/23 17:32:48 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Directories
AUTH_DIR      = $(COMMANDS_DIR)/auth

# Source files
AUTH_SRCS     = PassCommand.cpp \
				NickCommand.cpp \
				UserCommand.cpp

SRCS            += $(addprefix $(AUTH_DIR)/, $(AUTH_SRCS))


# VPATH
vpath %.cpp $(AUTH_DIR)
