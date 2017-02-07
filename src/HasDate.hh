<?hh

namespace Crashy;

use MongoDB\BSON\UTCDateTime;

trait HasDate
{
    protected UTCDateTime $on;

    public function getOn(): UTCDateTime
    {
        return $this->on;
    }
}
