NAME = ircserv

SRC_DIR = srcs/
OBJ_DIR = objs
INCLUDE_DIR = includes/

SRCS = 	\
		srcs/main.cpp \
		srcs/Client.cpp \
		srcs/Server.cpp \
		srcs/Command.cpp \
		srcs/Utils.cpp \
		srcs/commands/Invite.cpp \
		srcs/commands/Join.cpp \
		srcs/commands/Kick.cpp \
		srcs/commands/Mode.cpp \
		srcs/commands/Nick.cpp \
		srcs/commands/Part.cpp \
		srcs/commands/Pass.cpp \
		srcs/commands/PingPong.cpp \
		srcs/commands/Privmsg.cpp \
		srcs/commands/Quit.cpp \
		srcs/commands/Topic.cpp \
		srcs/commands/User.cpp 

HEADER_LIST =	\
				Client.hpp \
				Command.hpp \
				Server.hpp \
				Utils.hpp \
				commands/Invite.hpp \
				commands/Join.hpp \
				commands/Kick.hpp \
				commands/Mode.hpp \
				commands/Nick.hpp \
				commands/Part.hpp \
				commands/Pass.hpp \
				commands/PingPong.hpp \
				commands/Privmsg.hpp \
				commands/Quit.hpp \
				commands/Topic.hpp \
				commands/User.hpp 

HEADERS = ${HEADER_LIST:%=${INCLUDE_DIR}/%}

OBJECTS = ${SRCS:%.cpp=${OBJ_DIR}/%.o}

CPP = @c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3

# Couleurs
RED = \033[0;91m
GREEN = \033[0;33m
BLUE = \033[1;34m
YELLOW = \033[1;33m
NC = \033[0m 

all: ${NAME}

# Compilation de l'exécutable
${NAME}: ${OBJECTS}
	${CPP} ${CFLAGS} ${OBJECTS} -o ${NAME}
	@echo "${GREEN}✅ ${NAME} compiled !"

# Dépendance du header
${OBJECTS}: ${HEADERS}

# Règle pour compiler les fichiers objets dans le dossier objs
${OBJ_DIR}/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "${BLUE} ↳  🛠️  Compiling $< into $@...${NC}"
	${CPP} ${CFLAGS} -I ${INCLUDE_DIR}  -c $< -o $@

# Création du répertoire objs si non existant
${OBJ_DIR}:
	@echo "${YELLOW}📂 Creating object directory...${NC}"
	@mkdir -p ${OBJ_DIR}/srcs

# Nettoyage des fichiers objets
clean:
	@echo "${RED}🧹 Cleaning object directory...${NC}"
	@rm -rf ${OBJ_DIR}

# Nettoyage complet
fclean: clean
	@echo "${RED}🧹 Cleaning executable...${NC}"
	@rm -f ${NAME}

# Rebuild complet
re: fclean all

.PHONY: all clean fclean re
