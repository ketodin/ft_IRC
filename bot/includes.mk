# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    includes.mk                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaubry-- <jaubry--@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/14 17:50:57 by jaubry--          #+#    #+#              #
#    Updated: 2026/05/06 17:34:33 by jaubry--         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

INCDIR		= include

INCDIRS_BOT	:= $(INCDIR)

HEADERS		= $(shell find $(INCDIRS_BOT) -type f \( -name '*.h' -o -name '*.hpp' -o -name '*.inc' -o -name '*.tpp' \))
