<?hh

namespace Crashy;

use MongoDB\BSON\ObjectID;

trait HasId
{
    protected ObjectID $id;

    public function getId(): ObjectID
    {
        return $this->id;
    }
}
