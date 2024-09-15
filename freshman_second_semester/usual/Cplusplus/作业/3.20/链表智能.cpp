#include <iostream>
#define nullptr NULL

class Node;

class SmartPointer
{
  Node *pointer;
  int *ref_cnt;
public:
  SmartPointer()
  {
    pointer = nullptr;
    ref_cnt = nullptr;
  }
  SmartPointer(Node *p)
  {
    pointer = p;
    ref_cnt = new int(1);
  }

  // 需要完成的函数
  SmartPointer(const SmartPointer &sptr);
  void assign(const SmartPointer &sptr); // 指针赋值，将sptr赋值给本指针
  ~SmartPointer();            // 析构函数，注意：为通过测试样例，需要保证析构函数可以被重复调用
};

class Node
{
  int id;
public:
  Node(int id)
  {
    this->id = id;
  }
  ~Node()
  {
    std::cout << id << ' ';
  }
};



SmartPointer::SmartPointer(const SmartPointer &sptr)
{
  if(sptr.pointer == nullptr)
    {
      pointer = nullptr;
      ref_cnt = nullptr;
    }
    else
    {
      Node *p;
      q = new ref_cnt;
      p.pointer = sptr.pointer;
      *q = 1;
     (*ref_cnt)++;
    }
}

void SmartPointer::assign(const SmartPointer &sptr)
{
    if(sptr.pointer == nullptr)
    {
      if(pointer == nullptr)
      {
        pointer = nullptr;
        ref_cnt = nullptr;
      }
      else
      {
        if((*ref_cnt) == 1)
        {
          (*ref_cnt)--;
          Node *p = pointer;
          int *q = ref_cnt;
          pointer = nullptr;
          delete p;
          ref_cnt = nullptr;
          delete q;
        }
        else
        {
          (*ref_cnt)--;
          pointer = nullptr;
          ref_cnt = nullptr;
        }
      }
    }
    else
    {
      if(pointer == nullptr)
      {
        pointer = sptr.pointer;
        ref_cnt = sptr.ref_cnt;
        (*ref_cnt)++;
      }
      else
      {
        if((*ref_cnt) > 1)
        {
         (*ref_cnt)--;
          pointer = sptr.pointer;
          ref_cnt = sptr.ref_cnt;
          (*ref_cnt)++;
        }
        else
        {
          int *q = ref_cnt;
          (*ref_cnt)--;
          ref_cnt = sptr.ref_cnt;
          delete q;
          Node *p = pointer;
          pointer = sptr.pointer;
          delete p;
          (*ref_cnt)++;
        }
      }
    }
}

SmartPointer::~SmartPointer()
{
  if(pointer == nullptr)
    {
      pointer = nullptr;
      ref_cnt = nullptr;
    }
  else
    {
      if((*ref_cnt) == 1)
      {
      (*ref_cnt)--;
      Node *p = pointer;
      int *q = ref_cnt;
      pointer = nullptr;
      delete p;
      ref_cnt = nullptr;
      delete q;
      }
      else
      {
      (*ref_cnt)--;
      pointer = nullptr;
      ref_cnt = nullptr;
      }
    }         
}

