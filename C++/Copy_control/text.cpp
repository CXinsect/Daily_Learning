#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <sstream>
class QueryResult;

std::string make_plural (size_t ctr,const std::string &word,
                                    const std::string &ending)
{
    return (ctr > 1) ? word + ending : word;
}
class TextQuery {
    public:
        using lineNum = std::vector<std::string>::size_type;
        TextQuery(std::ifstream&);
        QueryResult query(const std::string&) const;
    private:
        std::shared_ptr<std::vector<std::string>> file;
        std::map<std::string,std::shared_ptr<std::set<lineNum>>>wordMap;
};

TextQuery::TextQuery(std::ifstream &is) :file(new std::vector<std::string>){
    std::string text = std::string();
    while(getline(is,text)) {
        file->push_back(text);
        int n = file->size() -1;
        std::istringstream line(text);
        std::string word;
        while(line >> word) {
            auto &lines = wordMap[word];
            if(!lines) {
                lines.reset(new std::set <lineNum>);
            }
            lines->insert(n);
        }
    }
}
class QueryResult {
    friend std::ostream& print(std::ostream&,const QueryResult&);
    public:
        QueryResult(std::string s,std::shared_ptr<std::set<TextQuery::lineNum>>p,
                                std::shared_ptr<std::vector<std::string>>f) :
                    sought(s),lines(p),file(f) {}
    private:
        std::string sought = std::string();
        std::shared_ptr <std::set<TextQuery::lineNum>> lines;
        std::shared_ptr <std::vector<std::string> >file;
};
QueryResult TextQuery::query(const std::string &sought) const {
    static std::shared_ptr <std::set<lineNum>> nodata(new std::set <lineNum>);
    auto ret = wordMap.find(sought);
    if(ret == wordMap.end()) 
        return QueryResult(sought,nodata,file);
    else
        return QueryResult(sought,ret->second,file);
    
}
std::ostream &print(std::ostream& os,const QueryResult &qr) {
    os << qr.sought << "occuers " << qr.lines->size() << " " <<make_plural(qr.lines->size(),"time","s") << std::endl;
    for(auto num : *qr.lines)
        os << "\t(line " << num + 1 << ")" << *(qr.file->begin() + num)<< std::endl;
    return os;

}

int main (int argc,char *argv[]) {
    std::ifstream input(argv[1]);
    TextQuery tq(input);
    while(true) {
        std::cout << "please enter the word " << std::endl;
        std::string s;
        if(!(std::cin >> s) || s == "q") break;
        print(std::cout,tq.query(s));
    }
    return 0;
}