#ifndef DATA_TYPE_H
#define DATA_TYPE_H

typedef struct DATA_TYPE
{
  enum
  {
    DATA_TYPE_STRING,
    DATA_TYPE_CHAR,
    DATA_TYPE_INT
  } type;

  int modifiers[3];
} data_type_T;

data_type_T* init_data_type();

#endif
