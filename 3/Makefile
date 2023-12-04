ifeq ($(DEBUG),1)
	override PREPROCESSOR_FLAGS += -Wp,-D,DEBUG
endif

all: main

main: main.cpp
	g++ $(PREPROCESSOR_FLAGS) main.cpp -o main

clean:
	rm main
