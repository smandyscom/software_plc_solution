

int	startIODevice();
int	closeIODevice();
int getGenericInput(void* data, const size_t fullSize);
int setGenericOut	(void* data, const size_t);
int getDedicatedInput(void* data, const size_t fullSize);
int setDedicatedOutput(void* data, const size_t fullSize);