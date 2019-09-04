#include "dataBase.h"

std::map<std::pair<std::string, long long>, std::string>::iterator
DataBase::FindString(const std::string &key) {
  typedef std::map<std::pair<std::string, long long>, std::string>::iterator
      Iterator;
  // std::map<std::pair<std::string, long long>, std::string>::iterator it =
  //     String_.begin();
  std::shared_ptr<Iterator> ptr(new Iterator);
  *ptr = String_.begin();
  while (*ptr != String_.end()) {
    if ((*ptr)->first.first == key) {
      break;
    } else
      (*ptr)++;
  }
  return (*ptr);
}
std::map<std::pair<std::string, long long>,
         std::multimap<std::string, std::string>>::iterator
DataBase::FindHash(const std::string &key) {
  // std::map<std::pair<std::string, long long>,
  //          std::map<std::string, std::string>>::iterator it = Hash_.begin();
  typedef std::map<std::pair<std::string, long long>,
                   std::multimap<std::string, std::string>>::iterator Iterator;
  std::shared_ptr<Iterator> ptr(new Iterator);
  *ptr = Hash_.begin();
  while (*ptr != Hash_.end()) {
    if ((*ptr)->first.first == key) {
      break;
    } else
      (*ptr)++;
  }
  return (*ptr);
}
std::map<std::pair<std::string, long long>, std::list<std::string>>::iterator
DataBase::FindList(const std::string &key) {
  // std::map<std::pair<std::string, long long>,
  //          std::list<std::string>>::iterator it = List_.begin();
  typedef std::map<std::pair<std::string, long long>,
                   std::list<std::string>>::iterator Iterator;
  std::shared_ptr<Iterator> ptr(new Iterator);
  *ptr = List_.begin();
  while (*ptr != List_.end()) {
    if ((*ptr)->first.first == key) {
      break;
    } else {
      size++;
      (*ptr)++;
    }
  }
  return (*ptr);
}
bool DataBase::addKeySpace(int type, int encoding, const std::string &key,
                           const std::string &value, const std::string &value1,
                           long long expiresTime) {
  type_ = type;
  encoding_ = encoding;
  key_ = key;
  value_ = value;
  value1_ = value1;
  if (type_ == DataStructure::ObjString) {
    std::map<std::pair<std::string, long long>, std::string>::iterator it =
        FindString(key);
    if (it == String_.end())
      String_.insert(make_pair(make_pair(key, expiresTime_), value_));
    else {
        String_.erase(it);
        String_.insert(make_pair(make_pair(key_,expiresTime+getTimestamp()),value_));
      // setKeySpaceExpiresTime(getTimestamp());
    }
  } else if (type == DataStructure::ObjHash) {
    std::map<std::pair<std::string, long long>,
             std::multimap<std::string, std::string>>::iterator it =
        FindHash(key);
    if (it == Hash_.end()) {
      std::multimap<std::string, std::string> tmp;
      tmp.insert(make_pair(value_, value1_));
      Hash_.insert(make_pair(make_pair(key_, expiresTime_), tmp));
    } else {
      std::multimap<std::string, std::string> tmp;
      std::multimap <std::string,std::string>::iterator iter = it->second.begin();
      std::cout << iter->first << " " << iter->second << std::endl;
      while(iter != it->second.end()) {
        tmp.insert(make_pair(iter->first,iter->second));
        iter++;
      }
      tmp.insert(make_pair(value,value1));

      Hash_.erase(it);
      std::cout << "getkeyspace size: " << tmp.size() << std::endl;
      Hash_.insert(make_pair(make_pair(key_, expiresTime + getTimestamp()), tmp));
      // std::map<std::string, std::string>::iterator iter = it->second.begin();
      // while (iter != it->second.end()) {
      //   if (iter->first == key)
      //     break;
      //   else {
      //     iter++;
      //   }
      // }
      // //此刻该键值肯定存在
      // iter->second = value;
    }
  } else if (type == DataStructure::ObjList) {
    std::map<std::pair<std::string, long long>,
             std::list<std::string>>::iterator it = FindList(key);
    //如果没有找到则加入新元素　否则更新键值
    if (it == List_.end()) {
      std::list<std::string> tmp;
      tmp.push_back(value_);
      List_.insert(make_pair(make_pair(key_, expiresTime_), tmp));
    } else {
      //更新list的值
      List_.erase(it);
      std::list<std::string> tmp;
      tmp.push_back(value_);
      List_.insert(make_pair(make_pair(key_, expiresTime+getTimestamp()), tmp));
      // std::list<std::string>::iterator iter = it->second.begin();
      // while(size-- > 0)
      //     iter++;
      // *iter = value;
    }
  } else {
    std::cout << "Unknown type" << std::endl;
    return false;
    // abort();
  }
  std::cout << "addKeySpace has been Executed" << std::endl;
  return true;
}
bool DataBase::delKeySpace(int type, const std::string &key) {
  if (type == DataStructure::ObjString) {
    std::map<std::pair<std::string, long long>, std::string>::iterator it =
        String_.begin();
    while (it != String_.end()) {
      if (it->first.first == key) {
        break;
      } else
        it++;
    }
    if (it == String_.end())
      std::cout << "Not Found" << std::endl;
    else {
      String_.erase(it);
      return true;
    }
  } else if (type == DataStructure::ObjHash) {
    std::map<std::pair<std::string, long long>,
             std::multimap<std::string, std::string>>::iterator it =
        Hash_.begin();
    while (it != Hash_.end()) {
      if (it->first.first == key)
        break;
      else {
        it++;
      }
    }
    if (it == Hash_.end()) {
      std::cout << "Not Found" << std::endl;
      abort();
    } else {
      Hash_.erase(it);
      return true;
    }
  } else if (type == DataStructure::ObjList) {
    std::map<std::pair<std::string, long long>,
             std::list<std::string>>::iterator it = List_.begin();
    while (it != List_.end()) {
      if (it->first.first == key)
        break;
      else {
        it++;
      }
    }
    if (it == List_.end()) {
      std::cout << "Not Found" << std::endl;
      abort();
    } else {
      List_.erase(it);
      return true;
    }
  } else {
    std::cout << "Unknown type" << std::endl;
    abort();
  }
  return false;
}
std::string DataBase::getKeySpace(int type, const std::string &key) {
  std::string ret = std::string();
  if (!judgeKeySpaceExpiresTime(type, key)) {
    if (type == DataStructure::ObjString) {
      std::map<std::pair<std::string, long long>, std::string>::iterator it =
          String_.begin();
      while (it != String_.end()) {
        if (it->first.first == key) {
          break;
        } else
          it++;
      }
      if (it == String_.end()) ret = "Not Found";
      // std::cout << "Not Found" << std::endl;
      else
        ret = it->second;
    } else if (type == DataStructure::ObjHash) {
      std::map<std::pair<std::string, long long>,
               std::multimap<std::string, std::string>>::iterator it =
          Hash_.begin();
      while (it != Hash_.end()) {
        if (it->first.first == key)
          break;
        else {
          it++;
        }
      }
      if (it == Hash_.end()) {
        std::cout << "Not Found" << std::endl;
        ret = "Not Found";
      } else {
        std::multimap<std::string, std::string> tmp = it->second;
        std::cout << "size: --" << tmp.size() << std::endl;
        char buf[1024] = {0};
        char * pbuf = buf;
        int len = 0;
        std::multimap<std::string, std::string>::iterator iter = it->second.begin();
        while(iter != it->second.end()) {
          std::cout << "multimap" << iter->first << std::endl;
          // auto pos = tmp.equal_range(iter->first);
          // int n = snprintf(pbuf,sizeof(buf)-len,"%s %s ",pos.first->first.c_str(),pos.first->second.c_str());
          // assert(n >= 0);
          // len += n;
          // while(++pos.first != pos.second) {
          //   n = snprintf(pbuf+len,sizeof(buf)-len,"%s ",pos.first->first.c_str());
          //   len += n;
          // }
          int n = snprintf(pbuf+len,sizeof(buf)-len,"%s %s ",iter->first.c_str(),iter->second.c_str());
          len += n;
          iter++;
        }
        
        ret = buf;
        // for(auto pos = iter->equa)
        // ret = iter->second;
      }
    } else if (type == DataStructure::ObjList) {
      std::map<std::pair<std::string, long long>,
               std::list<std::string>>::iterator it = List_.begin();
      while (it != List_.end()) {
        if (it->first.first == key)
          break;
        else {
          it++;
        }
      }
      if (it == List_.end()) {
        std::cout << "Not Found" << std::endl;
        ret = "Not Found";
      } else {
        std::list<std::string>::iterator iter = it->second.begin();
        ret = *iter;
      }
    } else {
      std::cout << "Unknown Type" << std::endl;
      ret = "Not Found";
    }
  } else {
    delKeySpace(type, key);
    ret = "The key has expired and has been deleted";
  }
  //   std::cout << "getKeySpace: " << ret << std::endl;
  return ret;
}

long long DataBase::getKeySpaceExpiresTime(int type, const std::string &key) {
  long long ret;
  if (type == DataStructure::ObjString) {
    std::map<std::pair<std::string, long long>, std::string>::iterator it =
        String_.begin();
    while (it != String_.end()) {
      if (it->first.first == key)
        break;
      else
        it++;
    }
    if (it == String_.end()) {
      ret = -2;
    } else
      ret = it->first.second;
  } else if (type == DataStructure::ObjHash) {
    std::map<std::pair<std::string, long long>,
             std::multimap<std::string, std::string>>::iterator it =
        Hash_.begin();
    while (it != Hash_.end()) {
      if (it->first.first == key)
        break;
      else
        it++;
    }
    if (it == Hash_.end())
      ret = -2;
    else
      ret = it->first.second;
  } else if (type == DataStructure::ObjList) {
    std::map<std::pair<std::string, long long>,
             std::list<std::string>>::iterator it = List_.begin();
    while (it != List_.end()) {
      if (it->first.first == key)
        break;
      else
        it++;
    }
    if (it == List_.end())
      ret = -2;
    else
      ret = it->first.second;
  } else {
    ret = -3;
  }
  return ret;
}

long long DataBase::RemainingSurvivalTime(int type, const std::string &key) {
  long long ret = getKeySpaceExpiresTime(type, key);
  assert(ret != -1);
  long long now = getTimestamp();
  return now - ret;
}
bool DataBase::judgeKeySpaceExpiresTime(int type, const std::string &key) {
  long long expire = getKeySpaceExpiresTime(type, key);
  if (expire <= -1)
    return false;
  else {
    long long now = getTimestamp();
    if (now > expire)
      return true;
    else
      return false;
  }
}
void DataBase::rdbLoad() {
  char buf[2 * 1024] = {0};
  std::string path = getcwd(buf, sizeof(buf));
  path += "/1.rdb";
  struct stat stat_;
  int ret = ::stat(path.c_str(), &stat_);
  if(ret < 0) {
    std::cout << "Rdb file does not exist" << std::endl;
    return;
  }
  std::ifstream in;
  in.open(path, std::ios::in);
  // if opening is successful
  if (in.is_open()) {
    while (!in.eof()) {
      in.read(buf, sizeof(buf));
    }
    std::cout << "data in buffer: " << buf << std::endl;
  }
  in.close();
  std::string data = buf;
  std::cout << "data: " << data.size() << std::endl;
  int pos;
  while (data.size()) {
    ret = data.find("DATABASE");
    pos = data.find("EXPIRETIME");
    db_num_ = atoi(InterceptString(data, ret + 8, pos).c_str());
    ret = data.find_first_of('^');
    expiresTime_ = atoi(InterceptString(data, pos + 10, ret).c_str());
    pos = data.find_first_of('!');
    type_ = atoi(InterceptString(data, ret + 1, pos).c_str());
    std::cout << "Type: " << type_ << std::endl;
    if (type_ == DataStructure::ObjString) {
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
      data = data.substr(pos + 1, data.size());
      continue;
    }
    if (type_ == DataStructure::ObjHash) {
      std::multimap<std::string, std::string> tmp;
      ret = data.find_first_of('#');
      mkLen_ = atoi(InterceptString(data, pos + 1, ret).c_str());
      key_ = data.substr(ret + 1, mkLen_);
      pos = data.find('!', ret);
      // Skip hash size
      ret = data.find('!', pos + 1);
      pos = data.find('@', ret);
      while (ret != -1 && pos != -1) {
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
    if (type_ == DataStructure::ObjList) {
      ret = data.find_first_of('@');
      mkLen_ = atoi(InterceptString(data, pos + 1, ret).c_str());
      key_ = data.substr(ret + 1, mkLen_);
      pos = data.find('!', ret);
      ret = data.find('!', pos + 1);
      ListSize_ = atoi(InterceptString(data, pos + 1, ret).c_str());
      std::list<std::string> tmp;
      while (ListSize_-- > 0) {
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