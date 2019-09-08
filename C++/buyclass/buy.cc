#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
class Quote {
    public:
        Quote() = default;
        Quote(const std::string &book, double sales_prices) :
                    book_(book),price_(sales_prices) {}
        std::string isbn() const { return book_; }

        virtual double net_price(std::size_t n) const 
            { return n*price_; }
        virtual ~Quote() = default;

        virtual Quote* clone () const & { return new Quote(*this); }
        virtual Quote* clone() const && { return new Quote(std::move(*this)); }
    private:
        std::string book_;
    protected:
        double price_ = 0.0; 
};
class Bulk_Quote : public Quote {
    public:
        Bulk_Quote() = default;
        Bulk_Quote(const std::string&, double,std::size_t,double);
        double net_price(std::size_t) const override;
        Bulk_Quote * clone() const & override { return new Bulk_Quote(*this); }
        Bulk_Quote* clone () const && override { return new Bulk_Quote(std::move(*this)); }
    private:
        std::size_t min_qty = 0;
        double discount = 0.0;
};
class Basket {
    public:
        void add_item(const std::shared_ptr<Quote> &sale) { items.insert(sale); }
        double total_price (std::ostream &) const;
    private:
        static bool compare (const std::shared_ptr<Quote> &lhs,
                             const std::shared_ptr<Quote> &rhs)
        { return lhs->isbn() < rhs->isbn(); }
        std::multiset<std::shared_ptr<Quote>,decltype(compare)*> items{compare};
        
        static double print_total(std::ostream &os, const Quote &item, size_t n) {
            double ret = item.net_price(n);
            os << "ISBN: " << item.isbn() << " #sold " << n << " total due " << ret << std::endl;
            return ret;
        }
};
double Basket::total_price(std::ostream & os) const {
    double sum = 0.0;
    for(auto iter = items.cbegin(); iter != items.cend();iter = items.upper_bound(*iter)) {
        sum += print_total(os,**iter,items.count(*iter));
    }
    os << "total Sale: " << sum << std::endl;
    return sum;
}

class QueryResult;

class TextQuery {
    public:
        using line_no = std::vector<std::string>::size_type;
        TextQuery(std::ifstream &);
        QueryResult query(const std::string &) const;
    private:
        std::shared_ptr <std::vector<std::string>> file;
        std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
};
TextQuery::TextQuery(std::ifstream &is) :file(new std::vector <std::string>) {
    std::string text;
    while(getline(is,text)) {
        file->push_back(text);
        int n = file->size()-1;
        std::istringstream line(text);
        std::string word;
        while(line >> word) {
            auto &lines = wm[word];
            if(!lines) {
                lines.reset(new std::set<line_no>);
            }
            lines->insert(n);
        }
    }
}
class QueryResult {
    public:
        using line_no = std::vector <std::string>::size_type;
        friend std::ostream& print(std::ostream& ,const QueryResult&);
        static std::string make_plural(size_t n, const std::string& word, const std::string& ending) {
            return n > 1 ? word + ending : word;
        }
    public:
        QueryResult(std::string s,
                    std::shared_ptr <std::set<line_no>> p,
                    std::shared_ptr<std::vector<std::string>> f) :
            sought(s),lines(p),file(f) {}
    private:
        std::string sought;
        std::shared_ptr <std::set<line_no>> lines;
        std::shared_ptr <std::vector<std::string>> file;
};
QueryResult TextQuery::query(const std::string &sought) const {
    static std::shared_ptr <std::set<line_no>> nodata(new std::set<line_no>);
    auto loc = wm.find(sought);
    if(loc == wm.end()) {
        return QueryResult(sought,nodata,file);
    } else {
        return QueryResult(sought,loc->second,file);
    }
}
std::ostream& print(std::ostream& os ,const QueryResult& qr) {
    os << qr.sought << " occurs " << qr.lines->size() << " " << QueryResult::make_plural(qr.lines->size(),"time","s") << std::endl;
    for(auto num: *qr.lines) {
        os << "\t (line  " << num + 1 << ")" << *(qr.file->begin() + num) << std::endl;
    }
    return os;
}
class Query_base {
        friend class Query;
    protected:
        using line_no = TextQuery::line_no;
        virtual ~Query_base() = default;
    private:
        virtual QueryResult eval (const TextQuery& ) const = 0;
        virtual std::string rep() const = 0;
};
class Query {
    friend Query operator ~ (const Query &);
    friend Query operator | (const Query&, const Query &);
    friend Query operator & (const Query& ,const Query &);
    public:
        Query(const std::string&);
        QueryResult eval (const TextQuery &t) const { return q->eval(t); }
        std::string rep() const { return q->rep(); }
    private:
        Query(std::shared_ptr <Query_base> query) : q(query) {}
        std::shared_ptr<Query_base> q;
};
std::ostream & operator << (std::ostream &os, const Query &query) {
    return os << query.rep() << std::endl;
}
class WordQuery :public Query_base {
    friend class Query;
    WordQuery()
}
