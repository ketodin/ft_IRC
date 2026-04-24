# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    srcs.mk                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/14 17:51:47 by jaubry--          #+#    #+#              #
#    Updated: 2026/04/24 22:15:16 by lcalero          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Source files
IRC_SRCS	= main.cpp \
			  Server.cpp \
			  Client.cpp \
			  utils.cpp
SRCS		+= $(addprefix $(SRCDIR)/, $(IRC_SRCS))

# MKs
MKS			= commands/commands.mk

include $(addprefix $(SRCDIR)/, $(MKS))

# VPATH
vpath %.cpp	$(SRCDIR)
