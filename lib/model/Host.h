// #include <Arduino.h>
// #pragma once
// class Host
// {
// private:
//     int _hostId;
//     String _hostname;
//     String _version;
//     IPAddress _localIp;
//     IPAddress _gatewayIp;
//     String _mac;

// public:
//     Host()
//     {
//         this->setId(hostId);
//         this->setName(hostname);
//         this->setVersion(version);
//         this->setLocalIp(localIp);
//         this->setGatewayIp(gatewayIp);
//         this->setMac(mac);
//     }
//     ~Host() {}

//     // Setters
//     void setId(int id)
//     {
//         _hostId = id;
//     }

//     void setName(String hostname)
//     {
//         _hostname = hostname;
//     }

//     void setVersion(String version)
//     {
//         _version = version;
//     }

//     void setLocalIp(IPAddress ip)
//     {
//         _localIp = ip;
//     }

//     void setGatewayIp(IPAddress ip)
//     {
//         _gatewayIp = ip;
//     }

//     void setMac(String mac)
//     {
//         _mac = mac;
//     }

//     // Getters
//     int getId()
//     {
//         return _hostId;
//     }

//     String getName()
//     {
//         return _hostname;
//     }

//     String getVersion()
//     {
//         return _version;
//     }

//     IPAddress getLocalIp()
//     {
//         return _localIp;
//     }

//     IPAddress getGatewayIp()
//     {
//         return _gatewayIp;
//     }

//     String getMac()
//     {
//         return _mac;
//     }
// };

// class HostException : virtual public std::exception
// {

// protected:
//     int error_number;     ///< Error number
//     String error_message; ///< Error message

// public:
//     /** Constructor (C++ STL string, int, int).
//      *  @param err_num Error number
//      *  @param msg The error message
//      */
//     explicit HostException(int err_num, const String &msg) : error_number(err_num), error_message(msg)
//     {
//     }

//     /** Destructor.
//      *  Virtual to allow for subclassing.
//      */
//     virtual ~HostException() throw() {}

//     /** Returns a pointer to the (constant) error description.
//      *  @return A pointer to a const char*. The underlying memory
//      *  is in possession of the HttpException object. Callers must
//      *  not attempt to free the memory.
//      */
//     virtual const char *what() const throw()
//     {
//         return error_message.c_str();
//     }

//     /** Returns error number.
//      *  @return #error_number
//      */
//     virtual int getErrorNumber() const throw()
//     {
//         return error_number;
//     }
// };

// // class GHMk1Host : virtual Host
// // {
// // };