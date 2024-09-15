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

  // ��Ҫ��ɵĺ���
  SmartPointer(const SmartPointer &sptr);
  void assign(const SmartPointer &sptr); // ָ�븳ֵ����sptr��ֵ����ָ��
  ~SmartPointer();            // ����������ע�⣺Ϊͨ��������������Ҫ��֤�����������Ա��ظ�����
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

