# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    srcs.mk                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lcalero <lcalero@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/14 17:51:47 by jaubry--          #+#    #+#              #
#    Updated: 2026/04/27 18:43:39 by lcalero          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Source files
IRC_SRCS	= main.cpp \
			  Server.cpp \
			  ServerException.cpp \
			  Client.cpp \
			  utils.cpp \
			  signals.cpp
SRCS		+= $(addprefix $(SRCDIR)/, $(IRC_SRCS))

# MKs
MKS			= commands/commands.mk

include $(addprefix $(SRCDIR)/, $(MKS))

# VPATH
vpath %.cpp	$(SRCDIR)
