//
// Created by bingjian on 2018/11/1.
//

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <StringUtil.h>
#include "File.h"
#include "Log.h"

namespace future {

#ifdef DC_REPORT_DEBUG
    bool File::MkPath(const std::string &path) {
      return true;
    }

    bool File::IsFileExist(const std::string &path) {
      return true;
    }

    bool File::ReName(const std::string &oldPath, const std::string &newPath) {
      return true;
    }

    bool File::RemoveFile(const std::string &path) {
      return true;
    }

    std::shared_ptr<std::list<std::string> > File::FileList(const std::string &path) {
      std::shared_ptr<std::list<std::string> > retList = std::make_shared<std::list<std::string> >();
      DIR *dir = opendir(path.c_str());
      if (nullptr == dir) {
        return retList;
      }

      std::string pathTmp;
      struct dirent *file;
      while ((file = readdir(dir)) != nullptr) {
        if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
          continue;
        }
        pathTmp.clear();
        pathTmp.append(file->d_name);
        retList->push_back(pathTmp);
      }

      closedir(dir);
      return retList;
    }
#endif

    bool File::ZeroFillFile(int fd, size_t startPos, size_t size) {
        if (fd < 0) {
            return false;
        }

        if (lseek(fd, startPos, SEEK_SET) < 0) {
            Error("fail to lseek fd[%d], error:%s", fd, strerror(errno));
            return false;
        }

        const char zeros[4096] = {0};
        while (size >= sizeof(zeros)) {
            if (write(fd, zeros, sizeof(zeros)) < 0) {
                Error("fail to write fd[%d], error:%s", fd, strerror(errno));
                return false;
            }
            size -= sizeof(zeros);
        }

        if (size > 0 && (write(fd, zeros, size) < 0)) {
            Error("fail to write fd[%d], error:%s", fd, strerror(errno));
            return false;
        }
        return true;
    }

    std::string File::GetFileName(const std::string &path) {
        std::size_t pos = path.find_last_of("/");
        if (pos == std::string::npos) {
            return path.substr(0, path.find_last_of("."));
        } else {
            std::string tmp = path.substr(pos + 1);
            return tmp.substr(0, tmp.find_last_of("."));
        }
    }

    std::string File::GetFileNameWithExt(const std::string &path) {
        std::size_t pos = path.find_last_of("/");
        if (pos == std::string::npos) {
            return path;
        } else {
            std::string ret = path.substr(pos + 1);
            return std::move(ret);
        }
    }

    std::shared_ptr<std::list<std::string> >
    File::Filter(const std::list<std::string> &files,
                 std::function<bool(const std::string &fileName)> filter) {
        std::shared_ptr<std::list<std::string> > ret = std::make_shared<std::list<std::string> >();
        for (std::list<std::string>::const_iterator iter = files.begin();
             iter != files.end(); iter++) {
            if (filter(*iter)) {
                ret->push_back(*iter);
            }
        }
        return ret;
    }

}