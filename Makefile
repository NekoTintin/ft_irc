NAME = ircserv

SRC_DIR = srcs/
OBJ_DIR = objs
INCLUDE_DIR = includes/

SRCS = main.cpp \

HEADER_LIST = irc.hpp

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
${OBJ_DIR}/%.o: %.cpp | ${OBJ_DIR}
	@echo "${BLUE} ↳  🛠️  Compiling $< into $@...${NC}"
	${CPP} ${CFLAGS} -I ${INCLUDE_DIR}  -c $< -o $@

# Création du répertoire objs si non existant
${OBJ_DIR}:
	@echo "${YELLOW}📂 Creating object directory...${NC}"
	@mkdir -p ${OBJ_DIR}
	@mkdir -p ${OBJ_DIR}/

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
