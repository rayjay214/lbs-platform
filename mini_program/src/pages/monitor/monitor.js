const app = getApp();
Page({
  data: {
    min: 0,
    max: 0,
    noDevice: false,
    noData: true,
    updateTimer: '',
    currentLongitude: 0,
    currentLatitude: 0,
    deviceList: [],
    deviceIndex: 0,
    currentDevice: {},
    circles: [],
    fence: false,
    markers: [],
    bgcSet: '',
    animation: '',
    showNav: false,
    cmdUrl: ""
  },
  
  onLoad(option) {
    const that = this;
    that.setData({
      bgcSet: 'background-color:#3174EC'
    })
    wx.setNavigationBarColor({
      frontColor: '#ffffff',
      backgroundColor: '#3174EC'
    })
    let dev_id = app.globalData.dev_id
    console.log('rayjay' + dev_id)
    if (dev_id) {
      this.getDeviceList(function (res) {
        for (let i = 0; i < res.length; i++) {
          if (res[i].devid === dev_id) {
            that.setData({
              deviceIndex: i
            })
            break;
          }
        }
        that.updateCurrentDevice(that.data.deviceIndex)
        that.data.updateTimer = setInterval(function () {
          that.updateCurrentDevice(that.data.deviceIndex)
        }, 10000)
        that.setData({
          updateTimer: that.data.updateTimer
        })
      })
    } else {
      that.updateCurrentDevice(that.data.deviceIndex)
      that.data.updateTimer = setInterval(function () {
        that.updateCurrentDevice(that.data.deviceIndex)
      }, 10000)
      that.setData({
        updateTimer: that.data.updateTimer
      })
    }
  },

  onShow () { 
    const that = this
    if (!this.data.updateTimer) {
        that.data.updateTimer = setInterval(function () {
          that.updateCurrentDevice(that.data.deviceIndex)
        }, 10000)
        that.setData({
          updateTimer: that.data.updateTimer
        })
      }
  },

  onHide() {
    clearInterval(this.data.updateTimer)
  },
  regionchange(e) {
    
  },
  onUnload () {
    
  },
 
  markertap(e) {
    for (let i = 0; i < this.data.deviceList.length; i++) {
      if(parseInt(this.data.deviceList[i].devid) === e.markerId) {
        this.data.deviceIndex = i
        this.data.currentDevice = this.data.deviceList[i]
        this.data.currentDevice.lng = this.data.currentDevice.longitude;
        this.data.currentDevice.lat = this.data.currentDevice.latitude;
      }
    }
    console.log(this.data.currentDevice)
    console.log('rayjay markertap' + e.markerId)
  },
  controltap(e) {
    console.log('rayjay controltap' + e.controlId)
  },
  tap (e) {
    console.log('rayjay tap' + e)
  },
  toAlarm () {
    wx.navigateTo({
      url: './../alarm/alarm'
    })
  },
  bindDevice() {
    wx.navigateTo({
      url: './../bind/bind'
    })
  },
  toDetail () {
    wx.navigateTo({
      url: '../detail/detail?dev_id=' + this.data.currentDevice.devid + '&heart_time=' + this.data.currentDevice.heart_time
    })
  },
  toPlayback () {
    wx.navigateTo({
      url: './../calendar/calendar?dev_id=' + this.data.currentDevice.devid
    })
  },
  navigateTo() {
    var latitude = this.data.currentDevice.lat;
    var longitude = this.data.currentDevice.lng;
    wx.openLocation({
      latitude,
      longitude,
    })
  },

  getDeviceList(cb) {
    console.log('rayjay get device list')
    const that = this;
    wx.request({
      url: 'https://www.yunjiwulian.com/ent/getRunInfoByEid',
      data: {
        map_type: 'amap',
        eid: app.globalData.eid,
        access_token: app.globalData.access_token
      },
      success: function (res) {
        cb(res.data.data)
      },
      fail: function (err) {
        console.log(err)
      }
    })
  },

  updateCurrentDevice (index) {
    const that = this;
    wx.request({
      url: 'https://www.yunjiwulian.com/ent/getRunInfoByEid',
      data: {
        map_type: 'amap',
        eid: app.globalData.eid,
        access_token: app.globalData.access_token
      },
      success: function (res) {
        if (res.data.data.length == 0) {
          that.setData({
            deviceList: [],
            noDevice: true,
            markers: [],
            circles: []
          })
          that.getWeChatLocation(
            function (location) {
              if (location.latitude && location.longitude) {
                that.setData({
                  currentLatitude: location.latitude,
                  currentLongitude: location.longitude
                })
              } else {
                that.setData({
                  currentLatitude: res.data.defaultpos.lat,
                  currentLongitude: res.data.defaultpos.lng
                })
              }
            }
          )
        } else {
          that.setData({
            noDevice: false,
            noData: false,
          })
          that.data.currentDevice = res.data.data[index]
          that.data.currentDevice.lng = that.data.currentDevice.longitude;
          that.data.currentDevice.lat = that.data.currentDevice.latitude;
          that.setData({
            deviceList: res.data.data,
            currentDevice: that.data.currentDevice,
            markers: []
          })
          app.globalData.dev_id = that.data.currentDevice.dev_id
          app.globalData.location = {
            lat: that.data.currentDevice.lat,
            lng: that.data.currentDevice.lng
          }
          // 添加marker
          console.log('rayjay add marker')
          console.log(that.data)
          for (let i = 0; i < that.data.deviceList.length; i++) {
            that.data.deviceList[i].dev_index = i
            let dev_status = that.data.deviceList[i].dev_status
            let iconPath
            if (dev_status === 'online') {
              iconPath = './../images/move.png';
            } else if (dev_status === 'offline') {
              iconPath = './../images/offline.png';
            } else if (dev_status === 'static') {
              iconPath = './../images/static.png';
            }
            let obj = {
              iconPath: iconPath,
              id: parseInt(that.data.deviceList[i].devid),
              latitude: that.data.deviceList[i].latitude,
              longitude: that.data.deviceList[i].longitude,
              width: 30,
              height: 30,
              rotate: parseInt(that.data.deviceList[i].course),
              anchor: {
                x: .5,
                y: .5
              }
            }
            that.data.markers.push(obj)
          };
          that.setData({
            markers: that.data.markers
          });
          
          that.getAddress({
            latitude: that.data.currentDevice.lat,
            longitude: that.data.currentDevice.lng
          }, function (e) {
            that.data.currentDevice.address = e
            that.setData({
              currentDevice: that.data.currentDevice,
            });
            if (e) {
              that.setData({
                showNav: true
              })
            } else {
              that.setData({
                showNav: false
              })
            }
          })
          
        }
      },
      fail: function (err) {
        wx.showToast({
          title: '请重新登陆',
          icon: 'none',
          duration: 1500
        })
      }
    })
  },

  toDeviceList() {
    wx.navigateTo({
      url: '../deviceList/deviceList'
    })
  },

  getAddress (obj, cb) {
    const url = 'https://cloudapi.gpsoo.net/other'
    let data = {
      method: 'get_location',
      lng: obj.longitude,
      lat: obj.latitude,
      map_type: 'amap',
    }
    wx.request({
      url: url,
      data: data,
      success: function (res) {
        if (res.data.ret === 0) {
          let address = res.data.address
          cb(address)
        } else {
          console.log('获取地址信息错误')
        }
      }
    })
  },
  getWeChatLocation (cb) {
    const that = this
    console.log('rayjay get weixin loc')
    wx.getLocation({
      type: 'wgs84',
      success: function (res) {
        console.log('rayjay loc')
        console.log(res)
        cb(res)
      },
      fail: function (err) {
        console.log(err)
        cb(err)       
      }
    })
  },
  toCommand () {
    let params = "account="+app.globalData.account+"&time="+ String(new Date().getTime()).substr(0, 10) +"&imei="+this.data.currentDevice.imei+"&name="+this.data.currentDevice.name+"&devtype="+this.data.currentDevice.dev_type+"&access_token="+app.globalData.accessToken+"&offline=" + String(this.data.currentDevice.device_info_new == 3);
    console.log(params);
    wx.navigateTo({
      url: "./../cmd/cmd?" + params
    })
  }
})