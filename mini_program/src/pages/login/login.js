// pages/login/login.js
import md5 from './../../utils/md5.js'
const app = getApp()
Page({
  /**
   * 页面的初始数据
   */
  data: {
    could_submit: false,
    account: '',
    pwd: '',
    account_typed: false,
    pwd_typed: false,
  },
  accountInput: function (e) {
    this.setData({ account: e.detail.value });
    this.setData({ account_typed: true });
    if (this.data.account_typed == true && this.data.pwd_typed == true) {
      this.setData({ could_submit: true });
    }

  },
  pwdInput: function (e) {
    this.setData({ pwd: e.detail.value });
    this.setData({ pwd_typed: true });
    if (this.data.account_typed == true && this.data.pwd_typed == true) {
      this.setData({ could_submit: true });
    }
  },
  submit: function (e) {
    //this.setData({ could_submit: false });
    let timestamp = Math.floor(new Date().getTime() / 1000)
    let temp = md5.hexMD5(e.detail.value.pwd)
    temp = temp + timestamp
    let signature = md5.hexMD5(temp)
    console.log(timestamp);
    console.log(e)
    wx.request({
      url: 'https://www.yunjiwulian.com/login/loginByEnt', 
      data: {
        login_name: e.detail.value.account,
        time: timestamp,
        signature: signature
      },
      header: {
        'content-type': 'application/json' // 默认值
      },
      success: function (res) {
        console.log(res);
        if (res.statusCode == 200) {
          if (res.data.errcode === 0) {
            app.globalData.eid = res.data.data.eid
            app.globalData.access_token = res.data.data.access_token
            wx.showToast({
              title: res.data.msg,
              icon: 'none',
              duration: 2000
            })
            setTimeout(() => {
              wx.redirectTo({
                url: './../monitor/monitor'
              })
            }, 2000);
          } else {
            wx.showModal({
              title: '登陆失败',
              content: res.data.msg
            })
          }
        } else {
          wx.showModal({
            title: '登陆失败',
            content: res.data.errcode + '登陆失败,请检查您的网络设置，稍后重试'
          })
        }
      }
    })
  },
  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
    if (this.data.no == '' || this.data.pwd == '') {
      this.setData({ disabled: true });
    } else {
      this.setData({ disabled: false });
    }
  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  }
})
