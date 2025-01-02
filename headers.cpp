#include "headers.h"

const char* categoryToString(category cat)
{
    switch (cat) {
    case commerce: return "commerce";
    case organization: return "organization";
    case network: return "network";
    case education: return "education";
    case goverment: return "goverment";
    case technology: return "technology";
    case store: return "store";
    case blog: return "blog";
    case law: return "law";
    case health: return "health";
    case app: return "app";
    case dev: return "dev";
    case news: return "news";
    case music: return "music";
    case other: return "other";
    default: return "unknow"; // 防止无效值
    }
}

// 从字符串中解析category
category stringToCategory(const std::string& categoryStr) {
    if (categoryStr == "commerce") return commerce;
    if (categoryStr == "organization") return organization;
    if (categoryStr == "network") return network;
    if (categoryStr == "education") return education;
    if (categoryStr == "goverment") return goverment;
    if (categoryStr == "technology") return technology;
    if (categoryStr == "store") return store;
    if (categoryStr == "blog") return blog;
    if (categoryStr == "law") return law;
    if (categoryStr == "health") return health;
    if (categoryStr == "app") return app;
    if (categoryStr == "dev") return dev;
    if (categoryStr == "news") return news;
    if (categoryStr == "music") return music;
    return other;
}
