//buffer.h

#define BUFFER_SIZE 80

typedef struct buffer buffer;

struct buffer{
  int nextIn;
  int nextOut;
  char bufferMem[BUFFER_SIZE];
  semaphore emptyBuffers;
  semaphore fullBuffers;
  buffer* nextBuffer;
};

void createBuffer(buffer* b, buffer* nextBuffer);
void depositBuf(buffer *b, char c);
char removeBuf(buffer* b);

