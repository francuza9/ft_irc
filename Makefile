NAME	=	ircserv
NAME_BONUS	=	ircserv_bonus

CXX	=	c++
CXXFLAGS	=	-Wall -Werror -Wextra -std=c++98 -g

SR	=	src/
CMD = Commands/
BNS = Bonus/

SRCS		=	main.cpp \
				Client.cpp \
				Server.cpp \
				UserInfo.cpp \
				Channel.cpp \
				$(CMD)JoinAndLeave.cpp \
				$(CMD)Message.cpp

SRCS_BONUS	=	$(BNS)Bot.cpp \
				$(BNS)mainBonus.cpp \
				$(BNS)Translating.cpp \
				$(BNS)CommandsBonusUtils.cpp \
				$(BNS)CommandsBonus.cpp

OBJS	=	$(SRCS:.cpp=.o)
OBJS_BONUS	=	$(SRCS_BONUS:.cpp=.o)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

all	: $(NAME) bonus

bonus : $(OBJS_BONUS)
	$(CXX) $(CXXFLAGS) $(OBJS_BONUS) -o $(NAME_BONUS)

clean:
	rm -rf $(OBJS)
	rm -rf $(OBJS_BONUS)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(NAME_BONUS)

.c.o:
	${CXX} ${CXXFLAGS} -c $< -o ${<:.c=.o}

re:	fclean all

.PHONY : all clean fclean re


