# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    includes.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/14 17:50:57 by jaubry--          #+#    #+#              #
#    Updated: 2026/04/14 17:51:14 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

INCDIR		= include
INCDIRS_IRC	:= $(INCDIR)

HEADERS		= $(shell find $(INCDIRS_IRC) -type f \( -name '*.h' -o -name '*.hpp' \))
