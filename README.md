# Majo🧙‍♀️

A memory profiler focusing on long-lived objects.

## Motivation

I created this gem because I wanted to reduce the maximum memory usage of a Ruby program.

The existing memory profiler, `memory_profiler` gem, focuses on allocated and retained memory. This tool is useful to reduce allocations and retain memory, but it is not suitable for reducing the maximum memory usage.

This gem solves this problem by focusing on long-lived objects. Long-lived objects are objects that are not garbage collected for a long time. These objects are the main cause of the maximum memory usage.

## Installation

Install the gem and add to the application's Gemfile by executing:

    $ bundle add majo --group development

If bundler is not being used to manage dependencies, install the gem by executing:

    $ gem install majo

## Usage

Wrap the code you want to profile with `Majo.run`.
Then, call `report` method to display the result.

```ruby
result = Majo.run do
  code_to_profile
end

result.report
```

### Options

You can pass the following options to `report` method.

| Name        | Description              | Default   | Type                                                                     |
| ----------- | ------------------------ | --------- | ------------------------------------------------------------------------ |
| `out`       | Output file or IO        | `$stdout` | `IO`, `String` or an object having `to_path` method (such as `Pathname`) |
| `formatter` | The format of the result | `:color`  | `:color`, or `:csv`                                                      |

For example:

```ruby
result = Majo.run do
  code_to_profile
end

result.report(out: "majo-result.csv", formatter: :csv)
```

### Result Example

The result contains only long-lived objects, which are collected by the major GC.

The example is as follows:

```
Total 15055372 bytes (159976 objects)

Memory by file
-----------------------------------
10431760 /path/to/gems/rbs-3.5.1/lib/rbs/parser_aux.rb
 1966348 /path/to/gems/rbs-3.5.1/lib/rbs/environment_loader.rb
  980344 /path/to/gems/rbs-3.5.1/lib/rbs/types.rb
(snip)

Memory by location
-----------------------------------
10431760 /path/to/gems/rbs-3.5.1/lib/rbs/parser_aux.rb:20
 1942812 /path/to/gems/rbs-3.5.1/lib/rbs/environment_loader.rb:159
  249920 /path/to/gems/rbs-3.5.1/lib/rbs/types.rb:994
(snip)

Memory by class
-----------------------------------
4274028 String
3556632 RBS::Location
2197840 Array
(snip)

Objects by file
-----------------------------------
109126 /path/to/gems/rbs-3.5.1/lib/rbs/parser_aux.rb
 20813 /path/to/gems/rbs-3.5.1/lib/rbs/types.rb
 12236 /path/to/gems/rbs-3.5.1/lib/rbs/environment.rb
(snip)

Objects by location
-----------------------------------
109126 /path/to/gems/rbs-3.5.1/lib/rbs/parser_aux.rb:20
  4458 /path/to/gems/rbs-3.5.1/lib/rbs/namespace.rb:24
  4132 /path/to/gems/rbs-3.5.1/lib/rbs/types.rb:374
(snip)

Objects by class
-----------------------------------
52495 Array
22435 RBS::Location
11144 RBS::TypeName
(snip)
```

The CSV format is as follows:

```csv
Object class path,Class path,Method ID,Path,Line,Alloc generation,Free generation,Memsize,Count
Hash,,_parse_signature,/path/to/gems/rbs-3.5.1/lib/rbs/parser_aux.rb,20,20,22,160,3
Hash,RBS::EnvironmentLoader,each_signature,/path/to/gems/rbs-3.5.1/lib/rbs/environment_loader.rb,159,20,22,160,1
Hash,,_parse_signature,/path/to/gems/rbs-3.5.1/lib/rbs/parser_aux.rb,20,21,23,160,1
(snip)
```

You can find the raw data in the CSV format. It is useful for further analysis. For example: [A spreadsheet for Majo result](https://docs.google.com/spreadsheets/d/1Qe6ZSJ58bNfLbA_eSuL9FJy89taNPt325qAJnLDorOE/edit?gid=533761210#gid=533761210)

The columns are as follows:

| Column name         | Description                                                        |
| ------------------- | ------------------------------------------------------------------ |
| `Object class path` | The class name of the allocated object                             |
| `Class path`        | The class name of the receiver of the method allocating the object |
| `Method ID`         | The method name allocating the object                              |
| `Path`              | The file path of the method allocating the object                  |
| `Line`              | The line number of the method allocating the object                |
| `Alloc generation`  | The GC generation number when the object is allocated              |
| `Free generation`   | The GC generation number when the object is freed                  |
| `Memsize`           | The memory size of the object in bytes                             |
| `Count`             | Number of objects allocated with the same conditions               |

## Name

The name "Majo" is a Japanese word `魔女` that means "witch". It is a wordplay on the word "major" in "major GC".

## Thanks

This gem is inspired by [memory_profiler](https://github.com/SamSaffron/memory_profiler) gem.

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake spec` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and the created tag, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/pocke/majo.
