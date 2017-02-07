<?hh

chdir(dirname(__DIR__));
require 'vendor/autoload.php';

$myUrl = (string) $_SERVER['REQUEST_URI'];

$hostname = $_ENV['MONGO_PORT_27017_TCP_ADDR'] ?? 'localhost';

$m = new MongoDB\Driver\Manager("mongodb://$hostname:27017", ['journal' => true]);

$rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY);
$wc = new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY, 1000);
$accessDao = new \Crashy\DataAccess($m, 'hhvm.accesses', Map{'typeMapRoot' => \Crashy\Access::class, 'readPreference' => $rp, 'writeConcern' => $wc});
$userDao = new \Crashy\DataAccess($m, 'hhvm.users', Map{'typeMapRoot' => \Crashy\User::class, 'readPreference' => $rp, 'writeConcern' => $wc});
$widgetDao = new \Crashy\DataAccess($m, 'hhvm.widgets', Map{'typeMapRoot' => \Crashy\Widget::class, 'readPreference' => $rp, 'writeConcern' => $wc});

$access = new \Crashy\Access($myUrl, [random_int(0, 25), random_int(26, 50), random_int(51, 75)]);
$accessDao->persist($access);

// I dunno. Let's query a bunch of times.
for ($i=0; $i<12; $i++) {
    $users = $userDao->query(['email' => 'nobody@example.com'])->toArray();
}
if (count($users) == 0) {
    $user = new \Crashy\User([
        'email' => 'nobody@example.com',
        'displayName' => 'Nobody Special',
        'password' => 'correct horse battery staple',
        'address' => ['country' => 'US', 'region' => 'MD', 'locality' => 'Columbia'],
        'lastUrl' => $myUrl,
    ]);
    $userDao->persist($user);
} else {
    $user = current($users);
    $userDao->update(['_id' => $user->getId()], ['$set' => ['lastUrl' => $myUrl]]);
}

// insert a random widget
$subs = Vector::fromItems(range(1, random_int(2, 10)))->map(function ($i) {
    return new \Crashy\SubWidget(
        bin2hex(random_bytes(100 + $i)),
        (float) (1 / $i),
        $i % 2 == 0,
    );
});
$widget = new \Crashy\Widget('test ' . microtime(), $user->getId(), $subs);
$widgetDao->persist($widget);
$widgets = $widgetDao->query([]);
$widgetCount = iterator_count($widgets);

// I dunno. Let's query a bunch of times.
for ($i=0; $i<12; $i++) {
    $results = new Vector($accessDao->query(['url' => $myUrl], ['sort' => ['on' => -1]]));
}

$xhp = <x:frag>
    <p>{count($results) . " accesses to $myUrl"}</p>
    <p>{"I've created $widgetCount total widgets"}</p>
    <ui:accesses caption="Last Ten Here" results={$results->slice(0, 10)} />
</x:frag>;

echo $xhp;
