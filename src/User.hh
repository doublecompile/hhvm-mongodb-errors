<?hh

namespace Crashy;

use MongoDB\BSON\ObjectID;
use MongoDB\BSON\UTCDateTime;
use MongoDB\BSON\Persistable;

class User implements Persistable, \JsonSerializable
{
    use HasId;
    use HasDate;

    private string $email;
    private string $displayName;
    private string $password;
    private ?Address $address;
    private ?string $lastUrl;

    public function __construct(KeyedTraversable<string,mixed> $values)
    {
        $this->id = new ObjectID();
        $this->on = new UTCDateTime();
        $map = new Map($values);
        $this->email = (string) ($map['email'] ?? '');
        $this->displayName = (string) ($map['displayName'] ?? '');
        $this->password = (string) password_hash((string) ($map['password'] ?? ''), PASSWORD_DEFAULT);
        $this->address = Address::from($map['address'] ?? null);
        $last = $map['lastUrl'] ?? null;
        $this->lastUrl = $last === null ? null : (string) $last;
    }

    public function getEmail(): string
    {
        return $this->email;
    }

    public function getDisplayName(): string
    {
        return $this->displayName;
    }

    public function getPasswordHash(): string
    {
        return $this->password;
    }

    public function getAddress(): ?Address
    {
        return $this->address;
    }

    public function getLastUrl(): ?string
    {
        return $this->lastUrl;
    }

    public function jsonSerialize(): mixed
    {
        return $this->bsonSerialize();
    }

    public function bsonSerialize(): mixed
    {
        return ['_id' => $this->id, 'on' => $this->on, 'email' => $this->email,
            'displayName' => $this->displayName, 'password' => $this->password,
            'address' => $this->address, 'lastUrl' => $this->lastUrl];
    }

    public function bsonUnserialize(array<arraykey,mixed> $data): void
    {
        foreach ($data as $k => $v) {
            if ($k === '_id') {
                $this->id = $v instanceof ObjectID ? $v : new ObjectID((string) $v);
            } elseif ($k === 'on') {
                $this->on = $v instanceof UTCDateTime ? $v : new UTCDateTime();
            } elseif ($k === 'email') {
                $this->email = (string) $v;
            } elseif ($k === 'displayName') {
                $this->displayName = (string) $v;
            } elseif ($k === 'password') {
                $this->password = (string) $v;
            } elseif ($k === 'address') {
                $this->address = Address::from($v);
            } elseif ($k === 'lastUrl') {
                $this->lastUrl = $v === null ? null : (string) $v;
            }
        }
    }
}
