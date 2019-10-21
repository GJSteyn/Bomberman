NAME = bomberman
DEPS = 	sfml \
		pkg-config \
		cmake \
		glew \
		glm \
		assimp \
		crap \


export PKG_CONFIG_PATH=~/.brew/lib/pkgconfig/:~/.brew/Cellar/openal-soft/1.19.1/lib/pkgconfig

all : brew brew_deps cmake $(NAME)

$(NAME) : 
	cd build && make
	cp ./build/app/bomberman ./bomberman

brew :
	@ bash ./scripts/install_brew.bash
	@ bash ./scripts/install_brew_deps.bash $(DEPS)


cmake :
	@ /bin/echo -n "Finding cmake..."
	@ command -v cmake >> /dev/null && echo "found" || ( echo "not found" && exit 1 );

	
	mkdir -p build
	cd build && cmake ..

clean :
	@ rm -rf $(NAME)
	@ if [ -d build ]; then cd build && make clean; fi

fclean :
	rm -rf build

re : fclean all

.PHONEY : all brew cmake clean fclean re test