#include <iostream>

using namespace std;

class Ploynomail
{
private:
    struct Node{
        double coef;
        int expn;
        Node * next;
        Node(const int& d1, const double& d2):coef(d1), expn(d2), next(NULL){}
    };
    Node *node = new Node(0, -1);
    Node *head = node;
    Node *tail = node;
    void clear(){
        Node *p = head;
        while(p){
            Node *tmp = p->next;
            delete p;
            p = tmp;
        }
    }
public:
    Ploynomail(const int len, const double *coefs, const int *expn){
        for(int i=0; i<len; i++){
            Node* node = new Node(*(coefs + i), *(expn + i));
            tail->next = node;
            tail = node;
        }
    }
    ~Ploynomail(){clear();}

    void destroy(){clear();};
    void print_ploy();
    int ploy_len();
    void addPoly(Ploynomail& ploy_B);
    void subPloy(Ploynomail& ploy_B);
    void multiplyPloy(Ploynomail& ploy_B);
    //int* get_coef();
};

/*inline int*
Ploynomail::get_coef(){
    int len = this->ploy_len();
    int a[len];
    for(p)
}*/

inline void
Ploynomail::print_ploy(){
    for(Node* p=head; p; p = p->next){
        if(p->coef != 0){
            cout <<p->coef <<'x' <<'^' <<p->expn;
            if(p->next)
                cout << ' ';
            else
                cout <<endl;
        }
    }
}

inline int
Ploynomail::ploy_len(){
    int len = 0;
    for(Node* p=head; p; p = p->next){
        if(p->coef != 0) len++;
    }
    return len;
}

inline void
Ploynomail::addPoly(Ploynomail& ploy_B){
    Node *p1 = head->next;
    Node *p1_prev = head;
    Node *p2 = ploy_B.head->next;
    while(true){
        if(p2){
            if(p1 &&(p1->expn == p2->expn)){
                p1->coef += p2->coef;
                p1_prev = p1_prev->next;
                p1 = p1->next;
                p2 = p2->next;
                //cout<<'1'<<endl;
            }
            else if(!p1 || (p1->expn > p2->expn)){
                Node *node = new Node(p2->coef, p2->expn);
                node->next = p1;
                p1_prev->next = node;
                p1_prev = node;
                p2 = p2->next;
                //cout<<'2'<<endl;
            }
            else if(p1->expn < p2->expn){
                p1_prev = p1_prev->next;
                p1 = p1->next;
                //cout<<'3'<<endl;
            }
        }
        else break;
    }
    //ploy_B.destroy();
}

inline void
Ploynomail::subPloy(Ploynomail& ploy_B){
    Node *p1 = head->next;
    Node *p1_prev = head;
    Node *p2 = ploy_B.head->next;
    while(true){
        if(p2){
            if(p1 &&(p1->expn == p2->expn)){
                p1->coef -= p2->coef;
                p1_prev = p1_prev->next;
                p1 = p1->next;
                p2 = p2->next;
            }
            else if(!p1 || (p1->expn > p2->expn)){
                Node *node = new Node(-p2->coef, p2->expn);
                node->next = p1;
                p1_prev->next = node;
                p1_prev = node;
                p2 = p2->next;
            }
            else if(p1->expn < p2->expn){
                p1_prev = p1_prev->next;
                p1 = p1->next;
            }
        }
        else break;
    }
    //ploy_B.destroy();
}

inline void
Ploynomail::multiplyPloy(Ploynomail& ploy_B){
    //Node *p1 = head->next;
    //Node *p1_prev = head;
    //Node *p2 = ploy_B.head->next;
    int len1 = this->ploy_len();
    int len2 = ploy_B.ploy_len();
    //cout<< len1 <<endl;
    double co1[len1]={0}, co2[len2]={0};
    int ex1[len1]={0}, ex2[len2]={0};
    int i=0;
    for(Node* p=head; p; p = p->next){
        if(p->coef != 0){
            co1[i] = p->coef;
            ex1[i] = p->expn;
            i++;
        }
    }
    //cout<< co1[0]<<' ' << ex1[0]<<endl;
    i = 0;
    for(Node* p=ploy_B.head; p; p = p->next){
        if(p->coef != 0){
            co2[i] = p->coef;
            ex2[i] = p->expn;
            i++;
        }
    }
    //Ploynomail *main_ = new Ploynomail(len1, co1, ex1);

    for(i=0; i<len2; i++){
        //this->print_ploy();
        double tmp_co[len1];
        int tmp_ex[len1];
        for(int j=0; j<len1; j++){
            tmp_co[j] = co1[j] * co2[i];
            tmp_ex[j] = ex1[j] + ex2[i];
        }
        Ploynomail tmp_B(len1, tmp_co, tmp_ex);
        //tmp_B.print_ploy();
        (*this).addPoly(tmp_B);
        //this->print_ploy();
    }
    //this->print_ploy();
}

int main()
{
    double c[3] = {1, 1};//{1, 1, 1, 1, 1, 1};
    double c2[3] = {1, 1};//{-1, -1};
    int d[3] = {1, 100};{};
    int d2[3] = {100, 200};
    Ploynomail ploy_1(3, c, d);
    //cout<< ploy_1.ploy_len()<<endl;
    Ploynomail ploy_2(3, c2, d2);
    cout <<"add:";
    ploy_1.addPoly(ploy_2);
    ploy_1.print_ploy();
    cout <<endl;
    cout <<"sub:";
    ploy_1.subPloy(ploy_2);
    ploy_1.print_ploy();
    cout <<endl;
    cout <<"mult:";
    ploy_1.multiplyPloy(ploy_2);
    ploy_1.print_ploy();
    cout <<endl;
    return 0;
}
