// pic-fuses-12H.js
// تولید خودکار از XML - خانواده‌ی 12H

export const picFuseDB_12H = {"12HV609":[{"name":"CONFIG0","default":1023,"bits":[],"navigation":[{"mask":7,"members":[{"value":0,"text":"LP"},{"value":1,"text":"XT"},{"value":2,"text":"HS"},{"value":3,"text":"EC_IO"},{"value":4,"text":"INTRC_IO"},{"value":5,"text":"INTRC_CLKOUT"},{"value":6,"text":"EXTRC_IO"},{"value":7,"text":"EXTRC_CLKOUT"}]},{"mask":8,"members":[{"value":0,"text":"Off"},{"value":8,"text":"On"}]},{"mask":16,"members":[{"value":0,"text":"On"},{"value":16,"text":"Off"}]},{"mask":32,"members":[{"value":0,"text":"Internal"},{"value":32,"text":"External"}]},{"mask":64,"members":[{"value":0,"text":"All"},{"value":64,"text":"Off"}]},{"mask":128,"members":[{"value":0,"text":"4MHZ"},{"value":128,"text":"8MHZ"}]},{"mask":768,"members":[{"value":0,"text":"Off"},{"value":512,"text":"On_run"},{"value":768,"text":"On"}]}]}],"12HV615":[{"name":"CONFIG0","default":1023,"bits":[],"navigation":[{"mask":7,"members":[{"value":0,"text":"LP"},{"value":1,"text":"XT"},{"value":2,"text":"HS"},{"value":3,"text":"EC_IO"},{"value":4,"text":"INTRC_IO"},{"value":5,"text":"INTRC_CLKOUT"},{"value":6,"text":"EXTRC_IO"},{"value":7,"text":"EXTRC_CLKOUT"}]},{"mask":8,"members":[{"value":0,"text":"Off"},{"value":8,"text":"On"}]},{"mask":16,"members":[{"value":0,"text":"On"},{"value":16,"text":"Off"}]},{"mask":32,"members":[{"value":0,"text":"Internal"},{"value":32,"text":"External"}]},{"mask":64,"members":[{"value":0,"text":"All"},{"value":64,"text":"Off"}]},{"mask":128,"members":[{"value":0,"text":"4MHZ"},{"value":128,"text":"8MHZ"}]},{"mask":768,"members":[{"value":0,"text":"Off"},{"value":512,"text":"On_run"},{"value":768,"text":"On"}]}]}]};

// Proxy برای دستگاه‌های بدون داده در این خانواده
export const picFuseDBProxy_12H = new Proxy(picFuseDB_12H, {
  get(target, prop) {
    if (prop in target) return target[prop];
    return [];
  }
});
