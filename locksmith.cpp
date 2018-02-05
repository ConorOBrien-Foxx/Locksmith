#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <algorithm>

int pop_from(std::vector<int> &vec) {
    int res = vec.back();
    vec.pop_back();
    return res;
}

int pop_from(std::vector<int> &vec, int def) {
    if(vec.size() == 0) {
        return def;
    }
    
    int res = vec.back();
    vec.pop_back();
    return res;
}

int sign(int x) {
    return (x > 0) - (x < 0);
}

// modified from http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::string read_file(const char *filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if(in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    throw errno;
}

class Locksmith {
private:
    std::deque<int> code;
    std::vector<int> stack;
    std::vector<int> locs;
    size_t ptr = 0;
    void step();
    void extended(int);
    bool running();
    int next();
    int pop();
    int peek();
    void push(int);
public:
    Locksmith(std::string);
    void run();
    void debug();
};

int Locksmith::next() {
    return code[ptr++];
}

int Locksmith::pop() {
    return pop_from(stack, 0);
}

int Locksmith::peek() {
    return stack.back();
}

void Locksmith::push(int el) {
    stack.push_back(el);
}

bool Locksmith::running() {
    return ptr < code.size();
}

void Locksmith::extended(int op) {
    if(op == 0) {
        push(stack.back());
    }
    else if(op == 1) {
        push(pop() + 10 * pop());
    }
    else if(op == 2) {
        int count = pop();
        int pos = stack.size() - count - 1;
        push(stack[pos]);
        stack.erase(stack.begin() + pos);
    }
    else if(op == 4) {
        push(stack.size());
    }
    else if(op == 5) {
        std::reverse(stack.begin(), stack.end());
    }
    else if(op == 6) {
        int max = pop();
        for(int i = 0; i < max; i++) {
            push(i);
        }
    }
    else if(op == 7) {
        pop();
    }
    else if(op == 8) {
        push(sign(pop()));
    }
    else if(op == 9) {
        push(pop() == pop());
    }
}

void Locksmith::step() {
    int op = next();
    
    if(op == 0) {
        push(next());
    }
    else if(op == 1) {
        push(pop() + pop());
    }
    else if(op == 2) {
        push(-pop());
    }
    else if(op == 3) {
        push(pop() * pop());
    }
    else if(op == 4) {
        int b = pop(), a = pop();
        push(a / b);
    }
    else if(op == 5) {
        locs.push_back(ptr);
    }
    else if(op == 6) {
        int loc = pop_from(locs);
        int top = peek();
        if(top) {
            ptr = loc - 1;
        }
    }
    else if(op == 7) {
        extended(next());
    }
    else if(op == 8) {
        push(getchar());
    }
    else if(op == 9) {
        putchar(pop());
    }
}

void Locksmith::run() {
    while(running()) {
        step();
    }
}

void Locksmith::debug() {
    std::cerr << ">> DEBUG <<" << std::endl;
    for(int el : stack) {
        std::cerr << el << std::endl;
    }
}

Locksmith::Locksmith(std::string str) {
    for(auto chr : str) {
        if(isdigit(chr)) {
            code.push_back(chr - 48);
        }
    }
}

int main(int argc, char** argv) {
    if(argc < 2) {
        std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
        return -1;
    }
    Locksmith inst (read_file(argv[1]));
    inst.run();
    if(argc >= 3) {
        inst.debug();
    }
}