#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t 	byte;
typedef uint16_t 	word;
typedef uint32_t 	dword;
typedef uint64_t 	qword;

typedef int8_t 		sbyte;
typedef int16_t 	sword;
typedef int32_t 	sdword;
typedef int64_t 	sqword;

typedef enum Boolean: _Bool {
	false = 0,
	true = 1
} bool;

enum MemberType: byte {
	MemberType_UNSIGNED_BYTE,
	MemberType_UNSIGNED_SHORT,
	MemberType_UNSIGNED_INTEGER,
	MemberType_UNSIGNED_LONG,

	MemberType_SIGNED_BYTE,
	MemberType_SIGNED_SHORT,
	MemberType_SIGNED_INTEGER,
	MemberType_SIGNED_LONG,

	MemberType_STRING,

	MemberType_FLOAT,
	MemberType_DOUBLE,

	MemberType_TYPE,
	MemberType_ARRAY,
	MemberType_CUSTOM
};

struct Member {
	enum MemberType	type;
	dword 			size;
	byte 			data[];
} __attribute__((packed));

struct MemberArray {
	dword 			count;
	struct Member 	**members;
};

extern struct MemberArray decode_packet(byte *, dword);
extern void free_member_array(struct MemberArray);

byte *readallbytes(char *filename, dword *nmemb)
{
	FILE *f = fopen(filename, "rb");
	if (f == NULL) return NULL;

	fseek(f, 0, SEEK_END);
	*nmemb = ftell(f);
	fseek(f, 0, SEEK_SET);

	byte *data = malloc(*nmemb);
	fread(data, *nmemb, 1, f);
	fclose(f);
	return data;
}

int main()
{
	dword siz = 0;
	byte *data = readallbytes("test.bin", &siz);
	struct MemberArray packet = decode_packet(data, siz);

	printf("%d\n", packet.count);
	for (int i = 0; i < packet.count; i++) {
		struct Member *member = packet.members[i];
		printf("%d %d\n", member->type, member->size);
		if (member->type == MemberType_STRING) {
			char *str = malloc(member->size);
			memcpy(str, member->data, member->size);
			str[member->size - 1] = '\0';
			printf("%s\n", str);
		}
	}

	free(data);
	free_member_array(packet);
}
