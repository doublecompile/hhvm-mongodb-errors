<?hh

namespace Crashy;

class Address implements \MongoDB\BSON\Persistable, \JsonSerializable
{
    private ImmMap<string,string> $values;

    private static ImmSet<string> $fields = ImmSet{'name', 'street', 'extended', 'locality', 'region', 'postal', 'country', 'county'};

    public function __construct(KeyedTraversable<string,string> $values)
    {
        $map = Map{};
        foreach ($values as $field => $v) {
            if (self::$fields->contains($field)) {
                $value = trim((string) $v);
                if (strlen($value) > 0) {
                    $map[$field] = $value;
                }
            }
        }
        $this->values = $map->immutable();
    }

    public static function from(mixed $value): ?Address
    {
        if ($value instanceof Address) {
            return $value;
        } elseif ($value instanceof KeyedTraversable) {
            return new Address($value);
        } elseif (is_object($value)) {
            return self::from(get_object_vars($value));
        }
        return null;
    }

    public function getName(): ?string
    {
        return $this->values['name'] ?? null;
    }
    public function getStreet(): ?string
    {
        return $this->values['street'] ?? null;
    }
    public function getExtended(): ?string
    {
        return $this->values['extended'] ?? null;
    }
    public function getLocality(): ?string
    {
        return $this->values['locality'] ?? null;
    }
    public function getCounty(): ?string
    {
        return $this->values['county'] ?? null;
    }
    public function getRegion(): ?string
    {
        return $this->values['region'] ?? null;
    }
    public function getPostal(): ?string
    {
        return $this->values['postal'] ?? null;
    }
    public function getCountry(): ?string
    {
        return $this->values['country'] ?? null;
    }

    public function bsonSerialize(): mixed
    {
        return $this->values->toArray();
    }

    public function bsonUnserialize(array<arraykey,mixed> $data): void
    {
        $map = Map{};
        foreach (self::$fields as $field) {
            $value = $data[$field] ?? null;
            if ($value !== null) {
                $map[$field] = (string) $value;
            }
        }
        $this->values = $map->toImmMap();
    }

    public function jsonSerialize(): mixed
    {
        return $this->values->toArray();
    }

    public function __toString(): string
    {
        return json_encode($this->values);
    }
}
