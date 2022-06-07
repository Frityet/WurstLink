#pragma once

#define super(T) $super_##T##$
#define inherit(T) union { struct T; struct T super(T); }

#define pure __attribute__((pure))
#define constructor __attribute__((constructor))
#define destructor __attribute__((destructor))
