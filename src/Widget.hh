<?hh

namespace Crashy;

use MongoDB\BSON\ObjectID;
use MongoDB\BSON\UTCDateTime;
use MongoDB\BSON\Persistable;

class Widget implements Persistable, \JsonSerializable
{
    use HasId;
    use HasDate;

    private string $name;
    private ObjectID $userId;

    private Vector<SubWidget> $subs;

    public function __construct(
        string $name,
        ObjectID $userId,
        Traversable<SubWidget> $subs,
    ) {
        $this->id = new ObjectID();
        $this->on = new UTCDateTime();
        $this->name = $name;
        $this->userId = $userId;
        $this->subs = new Vector($subs);
    }

    public function getName(): string
    {
        return $this->name;
    }

    public function getUserId(): ObjectID
    {
        return $this->userId;
    }

    public function getSubs(): \ConstVector<SubWidget>
    {
        return $this->subs->immutable();
    }

    public function jsonSerialize(): mixed
    {
        return $this->bsonSerialize();
    }

    public function bsonSerialize(): mixed
    {
        $doc = ['_id' => $this->id, 'on' => $this->on, 'user' => $this->userId];
        if ($this->subs) {
            $doc['subs'] = $this->subs->toArray();
        }
        return $doc;
    }

    public function bsonUnserialize(array<arraykey,mixed> $data): void
    {
        $subs = Vector{};
        foreach ($data as $k => $v) {
            if ($k === '_id') {
                $this->id = $v instanceof ObjectID ? $v : new ObjectID((string) $v);
            } elseif ($k === 'on') {
                $this->on = $v instanceof UTCDateTime ? $v : new UTCDateTime();
            } elseif ($k === 'user') {
                $this->userId = $v instanceof ObjectID ? $v : new ObjectID((string) $v);
            } elseif ($k === 'subs' && $v instanceof Traversable) {
                foreach ($v as $sub) {
                    $sub = SubWidget::from($sub);
                    if ($sub !== null) {
                        $subs[] = $sub;
                    }
                }
            }
        }
        $this->subs = $subs;
    }
}
