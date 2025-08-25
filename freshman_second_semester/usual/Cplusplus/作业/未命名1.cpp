#include <iostream>
#include <vector>
using namespace std;

// ATTENTION: you should not modify any of the following interfaces
class Polynomial {
  friend Polynomial add(const Polynomial &p1, const Polynomial &p2);
  friend Polynomial derivate(const Polynomial &p);

public:
  Polynomial() {}

  explicit Polynomial(const vector<int> &coefficients);
  Polynomial(const Polynomial &other);
public:
  void output() const;

private:
  vector<int> coefficients_; // bonus: maybe more efficient data structures?
  int len;
};


Polynomial::Polynomial(const vector<int> &coefficients) {
  coefficients_ = coefficients;
}

Polynomial::Polynomial(const Polynomial &other) {
  this->coefficients_ = other.coefficients_;
}

Polynomial add(const Polynomial &p1, const Polynomial &p2) {
   int len1 = p1.coefficients_.size();
   int len2 = p2.coefficients_.size();
    if(len1 > len2)
     {
       Polynomial A(p1);
       int max = p1.coefficients_.size() > p2.coefficients_.size() ? p1.coefficients_.size() : p2.coefficients_.size();
       for(int i= 0;i< max;i++)
       {
         if((p1.coefficients_[i] + p2.coefficients_[i]) == 0)
         {
           max--;
         }
       }
  for(int i = 0;i < max;i++)
  {
    A.coefficients_[i] = p1.coefficients_[i] + p2.coefficients_[i];
  }
  int valid = 0;
  for(int i = 0;i < max;i++)
  {
    if(A.coefficients_[i] != 0)
    {
      A.len = max - i;
      valid = 1;
      break;
    }
    if(valid == 0) A.len = 0;
  }
  return A;
     }
     else
     {
       Polynomial A(p2);
       int max = p1.coefficients_.size() > p2.coefficients_.size() ? p1.coefficients_.size() : p2.coefficients_.size();
  for(int i = 0;i < max;i++)
  {
    A.coefficients_[i] = p1.coefficients_[i] + p2.coefficients_[i];
  }
  int valid = 0;
  for(int i = 0;i < max;i++)
  {
    if(A.coefficients_[i] != 0)
    {
      A.len = max - i;
      valid = 1;
      break;
    }
    if(valid == 0) A.len = 0;
  }
  return A;
      }

}

Polynomial derivate(const Polynomial &p)
{
    Polynomial ans(p);
    int len = (ans.coefficients_).size();
    for (int i = len - 1; i > 0; i--)
    {
        (ans.coefficients_)[i] = (len - i) * (p.coefficients_)[i - 1];
    }
    (ans.coefficients_).erase((ans.coefficients_).begin() + 0);
    return ans;
}

void Polynomial::output() const
{
    int len = coefficients_.size();
    for (int i = 0; i < len; i++)
    {
        if (coefficients_[i] != 0)
            cout << coefficients_[i] << "(" << len - i - 1 << ")" << " ";
    }
    cout << endl;
}


int main(void)
{
Polynomial t({1, 1, 1}); // x^2 + x + 1
derivate(t).output(); // 2x + 1
} 

