// heap object - info pointer, payload

struct info_table {
  uint type;
  union info_table_u extra;
  struct layout layout;

};

struct layout {
  int num;
  stuct arg_entry * entries;
};

struct arg_entry {
  int size; // bytes
  bool pointer;
};

// pap's are not contained in the info table but in the payload itself
union info_table_u {
  struct fun function;
  struct con constructor;

};

struct fun {
  void (*code)();
  int arity;
};

struct con {
  int arity;
  int con_num;
};

struct pap {
  struct fun * ptr;
};


