#include <stdio.h>
#include <string.h>
#include <stdint.h>
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
	sdword 			size;
	byte 			data[];
} __attribute__((packed));

struct MemberArray {
	dword 			count;
	struct Member 	**members;
};


struct MemberArray decode_packet(data, size)
								 dword size;
								 byte data[static size];
{
	struct MemberArray array = {0};
	int offset = 0;

	struct Member *magic = malloc(sizeof(struct Member) + sizeof(qword));
	memcpy(magic, data, sizeof(*magic) + sizeof(qword));
	offset += sizeof(struct Member) + sizeof(qword); //13
	printf("%zu\n", sizeof(*magic));

	if ((magic->data[0] | (magic->data[1] << 8) | (magic->data[2] << 16) | (magic->data[3] << 24)) != 0xB00B135) {
		fprintf(stderr, "Invalid magic number! Expected 0xB00B135, got %d\n", (magic->data[0] | (magic->data[1] << 8) | (magic->data[2] << 16) | (magic->data[3] << 24)));
		free(magic);
		return array;
	}

	if (magic->type != MemberType_UNSIGNED_LONG) {
		fprintf(stderr, "Invalid magic type! Expected UNSIGNED_LONG (%d), got %d\n", MemberType_UNSIGNED_LONG, magic->type);
		return array;
	}

	struct Member *membercount = malloc(sizeof(struct Member) + sizeof(sdword));
	memcpy(membercount, data + offset, sizeof(*membercount) + sizeof(sdword));
	offset += sizeof(struct Member) + sizeof(sdword);
	if (membercount->type != MemberType_SIGNED_INTEGER)
		return array;

	array.count = membercount->data[0] | (membercount->data[1] << 8) | (membercount->data[2] << 16) | (membercount->data[3] << 24);;
	array.members = calloc(array.count, sizeof(struct Member *));

	array.members[0] = magic;
	array.members[1] = membercount;


	return array;
}

void free_member_array(struct MemberArray array)
{
	for (dword i = 0; i < array.count; i++) {
		free(array.members[i]);
	}
	free(array.members);
}
