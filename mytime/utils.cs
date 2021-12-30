using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TaironeMBSettings
{

    public static class utils
    {

        public static DateTime FromMyTime(this long myTime)
        {
            var epoch = new DateTime(2020, 1, 1, 0, 0, 0, DateTimeKind.Utc);
            return epoch.AddSeconds(myTime);
        }

        public static Int32 ToMyTime(this DateTime date)
        {
            var epoch = new DateTime(2020, 1, 1, 0, 0, 0, DateTimeKind.Utc);
            return (Int32)Convert.ToInt64((date - epoch).TotalSeconds);
        }
 
    }
}
