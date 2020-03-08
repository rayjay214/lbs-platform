#ifndef __gmqdirectI_h__
#define __gmqdirectI_h__

#include <stdint.h>

#include <Ice/Ice.h>
#include <gmqdirect.h>

namespace GMQDirect
{

class AppLocalWriter
{
public:

    virtual bool writeMessages(const ::std::pair< ::GMQDirect::Table::const_iterator,  ::GMQDirect::Table::const_iterator>&) = 0;

    virtual bool writeMessagesToIPPort(const ::std::pair< ::GMQDirect::Table::const_iterator,  ::GMQDirect::Table::const_iterator>&,
                                       const ::std::string&,
                                       const ::std::string&,
                                       ::Ice::Long) = 0;

    virtual bool writeMessagesFromTo(const ::std::pair< ::GMQDirect::Table::const_iterator,  ::GMQDirect::Table::const_iterator>&,
                                     const ::std::string&,
                                     const ::std::string&) = 0;

    virtual bool writeMessagesFrom(const ::std::string&,
                                   const ::std::pair< ::GMQDirect::Table::const_iterator,  ::GMQDirect::Table::const_iterator>&,
                                   const ::std::string&) = 0;
};

class GMQDirectSendI : virtual public GMQDirectSend
{
private:

    ::std::string gbr_gns_id, app_gns_id, forward_gmq_gns_id;
    ::std::string forward_gmq_ip;
    uint16_t forward_gmq_port;
    bool write_gbr;

    AppLocalWriter *app_local_writer_ptr;

    int __InitGMQDirectSendI(AppLocalWriter *_app_local_writer_ptr, const ::std::string &str_app_id,
        const ::std::string &str_gmq_ip,
        const uint16_t gmq_port, const ::std::string &str_gbr_id);

public:
    /* 
     * If app_gns_id or forward_gmq_ip equals to empty string '', then the library bypass gdd logic.
     * Thus, we are compatible with former machanism, in which user forward to GDD Service
     * by themselves
     */
    int InitGMQDirectSendI(AppLocalWriter *_app_local_writer_ptr, const ::std::string &str_app_id, const ::std::string &str_gmq_id,
        const ::std::string &str_gbr_id = "", bool write_gbr = false);
    
    int InitGMQDirectSendI(AppLocalWriter *_app_local_writer_ptr, const ::std::string &str_app_id, const ::std::string &str_gmq_ip,
        const uint16_t gmq_port, const ::std::string &str_gbr_id = "", bool write_gbr = false);

public:
    virtual bool writeMessages(const ::std::pair< ::GMQDirect::Table::const_iterator,  ::GMQDirect::Table::const_iterator>&,
                               const Ice::Current&);

    bool writeDuplication(const ::std::pair< ::GMQDirect::Table::const_iterator,  ::GMQDirect::Table::const_iterator> &,
       const ::std::string &);

    virtual bool writeMessagesToIPPort(const ::std::pair< ::GMQDirect::Table::const_iterator,  ::GMQDirect::Table::const_iterator>&,
                                       const ::std::string&,
                                       const ::std::string&,
                                       ::Ice::Long,
                                       const Ice::Current&);

    virtual bool writeMessagesFromTo(const ::std::pair< ::GMQDirect::Table::const_iterator,  ::GMQDirect::Table::const_iterator>&,
                                     const ::std::string&,
                                     const ::std::string&,
                                     const Ice::Current&);

    virtual bool writeMessagesFrom(const ::std::string&,
                                   const ::std::pair< ::GMQDirect::Table::const_iterator,  ::GMQDirect::Table::const_iterator>&,
                                   const ::std::string&,
                                   const Ice::Current&);
};

}

#define ICE_CONN_TIMEOUT 3000

#define ERR_ICE_CONN_URL -1
#define ERR_ICE_INVD_PROXY -2
#define ERR_ICE_NETWORK -3

#define ERR_GDD_NO_GMQ -101

#define STR_GDD_MAX_MSG "100000"
#define STR_GDD_MAX_THREAD "32"
#define STR_GDD_GBR_SUFFIX "_GBR"

template <class T>
static int GetICEClientPrx(::std::string &ICESrvIP, ::std::string &ICESrvObjId,
    uint16_t ICESrvPort, T &ICEClientPrx) {

    char ICEConnURL[2048] = {0};
    int r = snprintf(ICEConnURL, sizeof(ICEConnURL), "%s:tcp -h %s -p %d -t %d",
        ICESrvObjId.c_str(), ICESrvIP.c_str(), ICESrvPort, ICE_CONN_TIMEOUT);

    if (r <= 0 || r > (int)sizeof(ICEConnURL)) {

        return ERR_ICE_CONN_URL;
    } 

    Ice::PropertiesPtr props = Ice::createProperties();
    props->setProperty("Ice.MessageSizeMax", STR_GDD_MAX_MSG);
    props->setProperty("Ice.ThreadPool.Client.SizeMax", STR_GDD_MAX_THREAD);
    props->setProperty("Ice.ThreadPool.Server.SizeMax", STR_GDD_MAX_THREAD);
    Ice::InitializationData ice_init_data;
    ice_init_data.properties = props;
    Ice::CommunicatorPtr ic = Ice::initialize(ice_init_data);
    try
    {
        Ice::ObjectPrx base = ic->stringToProxy(std::string(ICEConnURL));
        ICEClientPrx = T::checkedCast(base);
        if(!ICEClientPrx)
        {

            return ERR_ICE_INVD_PROXY;
        }
    }
    catch(const Ice::Exception& ex)
    {

        return ERR_ICE_NETWORK;
    }

    return 0;
}

#endif

