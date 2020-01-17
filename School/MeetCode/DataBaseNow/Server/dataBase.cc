#include "dataBase.h"
#include "File.h"
#include "redisPersistence.h"

// #include "LRU.h"

bool DataBase::addKeySpace(int type, int encoding, const std::string &key,
                           const std::string &value, const std::string &value1,
                           long long expiresTime)
{
  type_ = type;
  encoding_ = encoding;
  key_ = key;
  value_ = value;
  value1_ = value1;
  int tempTime = expiresTime + getTimestamp();
  if (type_ == DataStructure::ObjString)
  {
    SMap::iterator Siter = sMap_.find(key);
    if (Siter == sMap_.end())
    {
      String_.insert(make_pair(make_pair(key, tempTime), value_));
      sMap_.insert(make_pair(key, tempTime));
      stringLru_.set(key, value);
    }
    else
    {
      String_.erase({key, sMap_[key]});
      sMap_.erase(key);
      sMap_.insert(make_pair(key, tempTime));
      String_.insert(make_pair(make_pair(key_, tempTime), value_));
      stringLru_.set(key, value);
    }
  }
  else if (type == DataStructure::ObjHash)
  {
    HMap::iterator Hiter = hMap_.find(key);
    if (Hiter == hMap_.end())
    {
      std::multimap<std::string, std::string> tmp;
      tmp.insert(make_pair(value_, value1_));
      Hash_.insert(make_pair(make_pair(key_, tempTime), tmp));
      hMap_.insert(make_pair(key, tempTime));
      hashLru_.set(key, tmp);
    }
    else
    {
      std::multimap<std::string, std::string> tmp;
      Hash::iterator it = Hash_.find({key, hMap_[key]});
      std::multimap<std::string, std::string>::iterator iter = it->second.begin();
      while (iter != it->second.end())
      {
        tmp.insert(make_pair(iter->first, iter->second));
        iter++;
      }
      tmp.insert(make_pair(value, value1));
      std::cout << "rmp: " << tmp.size() << std::endl;
      Hash_.erase({key, hMap_[key]});
      hMap_.erase(key);
      Hash_.insert(make_pair(make_pair(key, tempTime), tmp));
      hMap_.insert(make_pair(key, tempTime));
      hashLru_.set(key, tmp);
    }
  }
  else if (type == DataStructure::ObjList)
  {

    LMap::iterator lIter = lMap_.find(key);
    if (lIter == lMap_.end())
    {
      std::list<std::string> tmp;
      tmp.push_back(value_);
      List_.insert(make_pair(make_pair(key_, tempTime), tmp));
      lMap_.insert(make_pair(key, tempTime));
      listLRu_.set(key, tmp);
    }
    else
    {
      //更新list的值
      auto it = List_.find({key, lIter->second});
      std::list<std::string>::iterator iter = it->second.begin();
      std::list<std::string> tmp;
      while (iter != it->second.end())
      {
        tmp.push_back(*iter);
        iter++;
      }
      tmp.push_back(value_);
      List_.erase({key, lMap_[key]});
      lMap_.erase(key);
      std::cout << "ListObject size: " << tmp.size() << std::endl;
      List_.insert(make_pair(make_pair(key_, tempTime), tmp));
      lMap_.insert(make_pair(key, tempTime));
      listLRu_.set(key, tmp);
    }
  }
  else
  {
    std::cout << "Unknown type" << std::endl;
    return false;
  }
  std::cout << "addKeySpace has been Executed" << std::endl;
  return true;
}
bool DataBase::delKeySpace(int type, const std::string &key)
{
  if (type == DataStructure::ObjString)
  {
    SMap ::iterator hIter = sMap_.find(key);
    if (hIter != sMap_.end())
    {
      String_.erase({key, sMap_[key]});
      return true;
    }
    else
    {
      cout << "Not Found " << endl;
    }
  }
  else if (type == DataStructure::ObjHash)
  {
    HMap::iterator hIter = hMap_.find(key);
    if (hIter != hMap_.end())
    {
      Hash_.erase({key, hMap_[key]});
      return true;
    }
  }
  else if (type == DataStructure::ObjList)
  {
    LMap::iterator lIter = lMap_.find(key);
    if (lIter != lMap_.end())
    {
      List_.erase({key, lMap_[key]});
      return true;
    }
  }
  else
  {
    std::cout << "Unknown type" << std::endl;
    abort();
  }
  return false;
}
const std::string DataBase::delListObject(const std::string &key)
{
  auto lIter = lMap_.find(key);
  if (lIter != lMap_.end())
  {
    auto it = List_.find({key, lIter->second});
    std::string res = it->second.back();
    it->second.pop_back();
    return res;
  }
  else
  {
    return Status::NotFound("Not Found key").ToString();
  }
}
std::string DataBase::getKeySpace(int type, const std::string &key)
{
  std::string ret = std::string();
  if (!judgeKeySpaceExpiresTime(type, key))
  {
    if (type == DataStructure::ObjString)
    {
      string lruret = stringLru_.get(key);
      if (lruret != "")
      {
        return lruret;
      }
      else
      {
        SMap::iterator sIter = sMap_.find(key);
        if (sIter == sMap_.end())
        {
          auto ptr = shared_ptr<redisPersistence>();
          if(storage_.lock()) {
            auto ptr = storage_.lock();
            string fstr;
          }
          ret = Status::NotFound("Not Found").ToString();
        }
        else
        {
          auto it = String_.find({key, sIter->second});
          ret = '+' + it->second;
        }
      }
    }
    else if (type == DataStructure::ObjHash)
    {
      HMap::iterator hIter = hMap_.find(key);
      if (hIter == hMap_.end())
      {
        std::cout << "Not Found" << std::endl;
        ret = Status::NotFound("Not Found").ToString();
      }
      else
      {
        char buf[1024] = {0};
        char *pbuf = buf;
        auto it = Hash_.find({key, hIter->second});
        int n = 0, len = 0;
        for (auto iter = it->second.begin(); iter != it->second.end();
             iter = it->second.upper_bound(iter->first))
        {
          auto pos = it->second.equal_range(iter->first);
          for (auto i = pos.first; i != pos.second; i++)
          {
            std::cout << "test " << i->first << ": " << i->second << std::endl;
            n = snprintf(pbuf, sizeof(buf) - len, "%s %s ", i->first.c_str(), i->second.c_str());
            pbuf += n;
            len += n;
          }
        }
        //            while(iter != it->second.begin()) {
        //                if(tmpcount--)
        //                    iter = --it->second.end();
        //                else iter--;
        //                int count = it->second.count(iter->first);
        //                auto miter = it->second.find(iter->first);
        //                while(count) {
        //                    std::cout << "test " << miter->first << ": " << miter->second << std::endl;
        //                    n = snprintf(pbuf+n,sizeof(buf)-len,"%s %s ",miter->first.c_str(),miter->second.c_str());
        //                    len += n;
        //                    count--;
        //                    miter++;
        //                }
        //            }
        ret = buf;
        string t = "+";
        ret = t + ret;
        cout << ret << endl;
      }
    }
    else if (type == DataStructure::ObjList)
    {
      LMap::iterator lIter = lMap_.find(key);
      if (lIter == lMap_.end())
      {
        std::cout << "Not Found" << std::endl;
        ret = Status::NotFound("Not Found").ToString();
      }
      else
      {
        auto it = List_.find({key, lIter->second});
        std::list<std::string>::iterator iter = it->second.begin();
        ret = '+' + *iter;
      }
    }
    else
    {
      std::cout << "Unknown Type" << std::endl;
      ret = Status::InvalidArgument("InvaildArgument").ToString();
    }
  }
  else
  {
    delKeySpace(type, key);
    ret = "The key has expired and has been deleted";
  }
  return ret;
}

long long DataBase::getKeySpaceExpiresTime(int type, const std::string &key)
{
  long long ret;
  if (type == DataStructure::ObjString)
  {
    auto it = sMap_.find(key);
    if (it == sMap_.end())
      return -1;
    else
      ret = it->second;
  }
  else if (type == DataStructure::ObjHash)
  {
    auto it = hMap_.find(key);
    if (it == hMap_.end())
      return -3;
    else
      ret = it->second;
  }
  else if (type == DataStructure::ObjList)
  {
    auto it = lMap_.find(key);
    if (it == lMap_.end())
      ret = -2;
    else
      ret = it->second;
  }
  else
  {
    ret = -3;
  }
  return ret;
}

long long DataBase::RemainingSurvivalTime(int type, const std::string &key)
{
  long long ret = getKeySpaceExpiresTime(type, key);
  assert(ret != -1);
  long long now = getTimestamp();
  return now - ret;
}
bool DataBase::judgeKeySpaceExpiresTime(int type, const std::string &key)
{
  long long expire = getKeySpaceExpiresTime(type, key);
  if (expire <= -1)
    return false;
  else
  {
    long long now = getTimestamp();
    if (now > expire)
      return true;
    else
      return false;
  }
}
void DataBase::rdbLoad()
{
  char buf[2 * 1024] = {0};
  std::string path = getcwd(buf, sizeof(buf));
  path += "/1.rdb";
  struct stat stat_;
  int ret = ::stat(path.c_str(), &stat_);
  if (ret < 0)
  {
    cout << strerror(errno) << endl;
  }
  MmapFile mfile(static_cast<int>(stat_.st_size), path);

  mfile.MmapOpen();
  std::ifstream in;
  // in.open(path, std::ios::in);
  //if opening is successful
  // if (in.is_open()) {
  //   while (!in.eof()) {
  //     in.read(buf, sizeof(buf));
  //   }
  //   std::cout << "data in buffer: " << buf << std::endl;
  // }
  // in.close();

  std::string data(mfile.getFilePtr(), mfile.getFilePtr() + mfile.getFileSize());
  // string data = buf;
  std::cout << "data: " << data << data.size() << std::endl;
  int pos;
  while (data.size())
  {
    ret = data.find("FE");
    pos = data.find("FD");
    cout << pos << ret << endl;
    db_num_ = atoi(InterceptString(data, ret + 8, pos).c_str());
    ret = data.find_first_of('^');
    long long expiresTime_ = atoi(InterceptString(data, pos + 10, ret).c_str());
    pos = data.find_first_of('!');
    type_ = atoi(InterceptString(data, ret + 1, pos).c_str());
    std::cout << "Type: " << type_ << std::endl;
    if (type_ == DataStructure::ObjString)
    {
      ret = data.find_first_of('@');
      mkLen_ = atoi(InterceptString(data, pos + 1, ret).c_str());
      key_ = data.substr(ret + 1, mkLen_);
      pos = data.find('!', ret);
      ret = data.find('$', pos);
      valueLen_ = atoi(InterceptString(data, pos + 1, ret).c_str());
      value_ = data.substr(ret + 1, valueLen_);
      String_.insert(make_pair(make_pair(key_, expiresTime_), value_));
      // update string
      pos = data.find_first_of('\n');
      ret = data.find('\n', pos + 1);
      int len = InterceptString(data, pos + 1, ret + 1).size();
      data = data.substr(pos + 1, 94);
      continue;
    }
    if (type_ == DataStructure::ObjHash)
    {
      std::multimap<std::string, std::string> tmp;
      ret = data.find_first_of('#');
      mkLen_ = atoi(InterceptString(data, pos + 1, ret).c_str());
      key_ = data.substr(ret + 1, mkLen_);
      pos = data.find('!', ret);
      // Skip hash size
      ret = data.find('!', pos + 1);
      pos = data.find('@', ret);
      while (ret != -1 && pos != -1)
      {
        skeyLen_ = atoi(InterceptString(data, ret + 1, pos).c_str());
        skey_ = data.substr(pos + 1, skeyLen_);
        ret = data.find('!', pos);
        pos = data.find('$', ret);
        valueLen_ = atoi(InterceptString(data, ret + 1, pos).c_str());
        value_ = data.substr(pos + 1, valueLen_);
        tmp.insert(make_pair(skey_, value_));
        ret = data.find('!', pos + 1);
        pos = data.find('@', ret);
      }
      Hash_.insert(make_pair(make_pair(key_, expiresTime_), tmp));
      // update string
      pos = data.find_first_of('\n');
      ret = data.find('\n', pos + 1);
      int len = InterceptString(data, pos + 1, ret + 1).size();
      data = data.substr(pos + 1, data.size());
      continue;
    }
    if (type_ == DataStructure::ObjList)
    {
      ret = data.find_first_of('@');
      mkLen_ = atoi(InterceptString(data, pos + 1, ret).c_str());
      key_ = data.substr(ret + 1, mkLen_);
      pos = data.find('!', ret);
      ret = data.find('!', pos + 1);
      ListSize_ = atoi(InterceptString(data, pos + 1, ret).c_str());
      std::list<std::string> tmp;
      while (ListSize_-- > 0)
      {
        pos = data.find('$', ret);
        valueLen_ = atoi(InterceptString(data, ret + 1, pos).c_str());
        value_ = data.substr(pos + 1, valueLen_);
        tmp.push_back(value_);
        ret = data.find('!', pos);
      }
      List_.insert(make_pair(make_pair(key_, expiresTime_), tmp));
      // update string
      pos = data.find_first_of('\n');
      ret = data.find('\n', pos + 1);
      int len = InterceptString(data, pos + 1, ret + 1).size();
      data = data.substr(pos + 1, data.size());
      continue;
    }
    std::cout << "Finally: " << data << std::endl;
  }
}