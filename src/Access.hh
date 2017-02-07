<?hh

namespace Crashy;

use MongoDB\BSON\Persistable;
use MongoDB\BSON\ObjectID;
use MongoDB\BSON\UTCDateTime;

class Access implements \MongoDB\BSON\Persistable
{
    use HasId;
    use HasDate;

    private string $url;
    private Vector<int> $randoms;

    public function __construct(string $url, Traversable<int> $randoms)
    {
        $this->id = new ObjectID();
        $this->on = new UTCDateTime();
        $this->url = $url;
        $this->randoms = Vector::fromItems($randoms);
    }

    public function getUrl(): string
    {
        return $this->url;
    }

    public function getRandoms(): \ConstVector<int>
    {
        return $this->randoms->immutable();
    }

    public function bsonSerialize(): mixed
    {
        return ['_id' => $this->id, 'on' => $this->on, 'url' => $this->url, 'randoms' => $this->randoms->toArray()];
    }

    public function bsonUnserialize(array<arraykey,mixed> $data): void
    {
        $randoms = Vector{};
        foreach ($data as $k => $v) {
            if ($k === '_id') {
                $this->id = $v instanceof ObjectID ? $v : new ObjectID((string) $v);
            } elseif ($k === 'on') {
                $this->on = $v instanceof UTCDateTime ? $v : new UTCDateTime();
            } elseif ($k === 'url') {
                $this->url = (string) $v;
            } elseif ($k === 'randoms' && $v instanceof Traversable) {
                foreach ($v as $num) {
                    $randoms[] = (int) $num;
                }
            }
        }
        $this->randoms = $randoms;
    }
}
