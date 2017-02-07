<?hh // strict

class :ui:accesses extends :x:element
{
    category %flow;
    children (pcdata | %flow)*;
    attribute
        :table,
        ?Stringish caption,
        Traversable<Crashy\Access> results = [];

    protected function render(): XHPRoot
    {
        $tbody = <tbody/>;
        foreach ($this->:results as $row) {
            $tbody->appendChild(
                <tr>
                    <th scope="row">{(string) $row->getId()}</th>
                    <td>{$row->getUrl()}</td>
                    <td>{$row->getOn()->toDateTime()->format('c')}</td>
                    <td>{implode(', ', $row->getRandoms())}</td>
                </tr>
            );
        }
        $caption = $this->:caption ? <caption>{$this->:caption}</caption> : null;
        return <table>
            {$caption}
            <thead>
                <tr>
                    <th scope="col">ID</th>
                    <th scope="col">URL</th>
                    <th scope="col">On</th>
                    <th scope="col">Randoms</th>
                </tr>
            </thead>
            {$tbody}
        </table>;
    }
}
